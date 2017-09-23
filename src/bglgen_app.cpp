#include "bglgen_app.hpp"
#include "bglgen_lua.hpp"
#include "bglgen_blt.hpp"
#include "ids.hpp"
#include "version.hpp"
#include "gl_registry.hpp"
#include <be/core/version.hpp>
#include <be/core/alg.hpp>
#include <be/core/logging.hpp>
#include <be/core/log_exception.hpp>
#include <be/core/console_log_sink.hpp>
#include <be/core/lua_modules.hpp>
#include <be/core/service_simple_thread_pool_executor.hpp>
#include <be/core/service_simple_inline_executor.hpp>
#include <be/util/path_glob.hpp>
#include <be/util/get_file_contents.hpp>
#include <be/util/put_file_contents.hpp>
#include <be/util/line_endings.hpp>
#include <be/util/lua_modules.hpp>
#include <be/cli/cli.hpp>
#include <be/blt/lua_modules.hpp>
#include <be/belua/lua_helpers.hpp>
#include <be/belua/log_exception.hpp>
#include <be/sqlite/static_stmt_cache.hpp>
#include <be/sqlite/transaction.hpp>
#include <be/sqlite/log_exception.hpp>

namespace be::bglgen {
namespace {

///////////////////////////////////////////////////////////////////////////////
S inflate_bglgen_core() {
#ifdef BE_BGLGEN_COMPILED_LUA_MODULE_UNCOMPRESSED_LENGTH
   Buf<const UC> data = make_buf(BE_BGLGEN_COMPILED_LUA_MODULE, BE_BGLGEN_COMPILED_LUA_MODULE_LENGTH);
   return util::inflate_string(data, BE_BGLGEN_COMPILED_LUA_MODULE_UNCOMPRESSED_LENGTH);
#else
   return S(BE_BGLGEN_COMPILED_LUA_MODULE, BE_BGLGEN_COMPILED_LUA_MODULE_LENGTH);
#endif
}

///////////////////////////////////////////////////////////////////////////////
S inflate_bgl_default_template() {
#ifdef BE_BGLGEN_BGL_DEFAULT_TEMPLATE_UNCOMPRESSED_LENGTH
   Buf<const UC> data = make_buf(BE_BGLGEN_BGL_DEFAULT_TEMPLATE, BE_BGLGEN_BGL_DEFAULT_TEMPLATE_LENGTH);
   return util::inflate_string(data, BE_BGLGEN_BGL_DEFAULT_TEMPLATE_UNCOMPRESSED_LENGTH);
#else
   return S(BE_BGLGEN_BGL_DEFAULT_TEMPLATE, BE_BGLGEN_BGL_DEFAULT_TEMPLATE_LENGTH);
#endif
}

//////////////////////////////////////////////////////////////////////////////
void load_bgl_default_template(lua_State* L) {
   S bgl_default_template = inflate_bgl_default_template();
   lua_getglobal(L, "register_template_string");
   lua_pushlstring(L, bgl_default_template.c_str(), ( int ) bgl_default_template.size());
   lua_pushstring(L, "bgl_default");
   belua::ecall(L, 2, 0);
}

//////////////////////////////////////////////////////////////////////////////
Path lua_get_path(lua_State* L, const char* global) {
   Path p;
   if (LUA_TSTRING == lua_getglobal(L, global)) {
      std::size_t len;
      const char* path = lua_tolstring(L, -1, &len);
      p = Path(path, path + len);
   }
   lua_pop(L, 1);
   return p;
}

///////////////////////////////////////////////////////////////////////////////
void find_symbols_in_file(const Path& path, std::unordered_multimap<S, SymbolUsage>& symbols, I8& status) {
   be_short_debug("") << "Parsing source file: " << path.generic_string() | default_log();
   try {
      S data = util::get_text_file_contents_string(path);
      std::unordered_multimap<S, SymbolUsage> local_symbols;
      Lexer lex(path, data, local_symbols);
      lex();

      if (!local_symbols.empty()) {
         // post the results back to the main thread's symbols map
         service<SimpleInlineExecutor>(ids::bglgen_service_simple_inline_executor_foreground).post(
            [&symbols, s { std::move(local_symbols) }]() {
               symbols.insert(s.begin(), s.end());
            });
      }
      return;
   } catch (const fs::filesystem_error& e) {
      log_exception(e);
   } catch (const std::system_error& e) {
      log_exception(e);
   } catch (const std::exception& e) {
      log_exception(e);
   }

   // an error occurred, set status flag
   service<SimpleInlineExecutor>(ids::bglgen_service_simple_inline_executor_foreground).post(
      [&status]() { status = 3; });
}

///////////////////////////////////////////////////////////////////////////////
void find_symbols_in_dir(const Path& path, const S& extensions_regex, util::PathMatchType type, std::unordered_multimap<S, SymbolUsage>& symbols, I8& status) {
   try {
      Path base = fs::absolute(path);
      if (fs::exists(base)) {
         std::vector<Path> paths = util::greb(".*\\.(" + extensions_regex + ")", base, type);
         if (!paths.empty()) {
            auto& bg_executor = service<SimpleThreadPoolExecutor>(ids::bglgen_service_simple_thread_pool_executor_background);
            for (Path& p : paths) {
               be_short_debug("") << "Parsing source file: " << p.generic_string() | default_log();
               bg_executor.post([&,p]() { find_symbols_in_file(p, symbols, status); });
            }
         }
      } else {
         throw fs::filesystem_error("Path does not exist!", std::make_error_code(std::errc::not_a_directory));
      }
      return;
   } catch (const fs::filesystem_error& e) {
      log_exception(e);
   } catch (const std::system_error& e) {
      log_exception(e);
   } catch (const std::exception& e) {
      log_exception(e);
   }

   // an error occurred, set status flag
   service<SimpleInlineExecutor>(ids::bglgen_service_simple_inline_executor_foreground).post(
      [&status]() { status = 3; });
}

///////////////////////////////////////////////////////////////////////////////
void add_gl_data(lua_State* L, const char* global_name, bool sort_by_weight, std::unordered_map<S, U32>& map, std::unordered_multimap<S, SymbolUsage>& multimap) {
   std::vector<S> sorted;
   sorted.reserve(map.size());
   for (auto& sym : map) {
      sorted.push_back(sym.first);
   }

   if (!sort_by_weight) {
      std::sort(sorted.begin(), sorted.end());
   } else {
      std::sort(sorted.begin(), sorted.end(), [&](const S& a, const S& b) {
         U32 aw = map[a];
         U32 bw = map[b];

         if (aw != bw) {
            return aw > bw;
         }

         return a < b;
      });
   }

   lua_settop(L, 0);
   lua_createtable(L, ( int ) sorted.size(), 0); // 1

   U32 symbol_index = 1;
   for (auto& sym : sorted) {
      U32 weight = map[sym];

      lua_createtable(L, 0, 4); // 2

      lua_pushliteral(L, "name"); // 3
      lua_pushstring(L, sym.c_str()); // 4
      lua_rawset(L, 2);

      lua_pushliteral(L, "weight"); // 3
      lua_pushinteger(L, weight); // 4
      lua_rawset(L, 2);

      auto range = multimap.equal_range(sym);
      U32 n_refs = ( U32 ) std::distance(range.first, range.second);

      lua_pushliteral(L, "refs"); // 3
      lua_createtable(L, n_refs, 0); // 4

      U32 ref_index = 1;
      while (range.first != range.second) {
         const SymbolUsage& usage = range.first->second;
         lua_createtable(L, 0, usage.check.empty() ? 3 : 4); // 5

         lua_pushliteral(L, "path"); // 6
         lua_pushstring(L, usage.path.generic_string().c_str()); // 7
         lua_rawset(L, 5);

         lua_pushliteral(L, "line"); // 6
         lua_pushinteger(L, usage.line); // 7
         lua_rawset(L, 5);

         lua_pushliteral(L, "weight"); // 6
         lua_pushinteger(L, usage.weight); // 7
         lua_rawset(L, 5);

         if (!usage.check.empty()) {
            lua_pushliteral(L, "check"); // 6
            lua_pushstring(L, usage.check.c_str()); // 7
            lua_rawset(L, 5);
         }

         lua_rawseti(L, 4, ref_index);
         ++ref_index;
         ++range.first;
      }

      lua_rawset(L, 2);

      lua_rawseti(L, 1, symbol_index);
      ++symbol_index;
   }

   lua_setglobal(L, global_name);
}

///////////////////////////////////////////////////////////////////////////////
sqlite::StaticStmtCache* cache = nullptr;

/////////////////////////////////////////////////////////////////////////////////
// gl_registry.query('SELECT ...', { paramA, paramB, ... }, function (columnA, columnB, ... ) ... end) -> (nil)
// gl_registry.query('SELECT ...', paramA, paramB) -> singleColumnSingleRowResult
// gl_registry.query('SELECT ...', paramA, paramB) -> singleRowColumnA, singleRowColumnB, ...
// gl_registry.query('SELECT ...', paramA, paramB) -> { { rowAcolumnA, rowAcolumnB, ... }, { rowBcolumnA, ... }, ... }
int gl_registry_query(lua_State* L) {
   if (!cache) {
      luaL_error(L, "GL registry not yet initialized!");
   }

   const char* query = luaL_checkstring(L, 1);
   auto stmt = cache->obtain(query);

   if (lua_gettop(L) >= 2 && lua_type(L, 2) == LUA_TTABLE) {
      for (int i = 1, n = (int)lua_rawlen(L, 2); i <= n; ++i) {
         switch (lua_rawgeti(L, 2, i)) {
            case LUA_TBOOLEAN:
               stmt.bind(( int ) i, 0 != lua_toboolean(L, -1));
               break;
            case LUA_TSTRING:
               stmt.bind(( int ) i, lua_tostring(L, -1));
               break;
            case LUA_TNUMBER:
               if (0 != lua_isinteger(L, -1)) {
                  stmt.bind(( int ) i, lua_tointeger(L, -1));
               } else {
                  stmt.bind(( int ) i, lua_tonumber(L, -1));
               }
               break;
            default:
               stmt.bind(( int ) i);
               break;
         }
         lua_pop(L, 1);
      }

      if (lua_gettop(L) >= 3 && lua_type(L, 3) == LUA_TFUNCTION) {
         int cols = stmt.columns();
         lua_checkstack(L, cols);

         while (stmt.step()) {

            lua_pushvalue(L, 3);

            for (int i = 0; i < cols; ++i) {
               switch (stmt.get_type(i)) {
                  case sqlite::ResultType::blob:
                  case sqlite::ResultType::text:
                  {
                     std::size_t len;
                     const char* data = static_cast<const char*>(stmt.get_blob(i, len));
                     lua_pushlstring(L, data, len);
                     break;
                  }
                  case sqlite::ResultType::integer:
                     lua_pushinteger(L, stmt.get_i64(i));
                     break;
                  case sqlite::ResultType::real:
                     lua_pushnumber(L, stmt.get_f64(i));
                     break;
                  default:
                     lua_pushnil(L);
                     break;
               }
            }

            lua_call(L, cols, 1);

            if (lua_type(L, -1) == LUA_TBOOLEAN && 0 == lua_toboolean(L, -1)) {
               lua_pop(L, 1);
               break;
            }

            lua_pop(L, 1);
         }

         return 0;
      }
   } else {
      int i = 1;
      for (int slot = 2, n = lua_gettop(L); slot <= n; ++slot) {
         switch (lua_type(L, slot)) {
            case LUA_TBOOLEAN:
               stmt.bind(( int ) i, 0 != lua_toboolean(L, slot));
               break;
            case LUA_TSTRING:
               stmt.bind(( int ) i, lua_tostring(L, slot));
               break;
            case LUA_TNUMBER:
               if (0 != lua_isinteger(L, -1)) {
                  stmt.bind(( int ) i, lua_tointeger(L, slot));
               } else {
                  stmt.bind(( int ) i, lua_tonumber(L, slot));
               }
               break;
            default:
               stmt.bind(( int ) i);
               break;
         }
         ++i;
      }
   }

   lua_settop(L, 0);
   lua_pushnil(L); // 1; placeholder for result table if we have more than one row

   int cols = stmt.columns();
   int row = 0;
   while (stmt.step()) {
      ++row;

      if (row == 2) {
         // second row, create result table
         lua_createtable(L, 3, 0);
         lua_replace(L, 1); // 1 was nil

         lua_createtable(L, cols, 0);
         lua_replace(L, 2); // 2 was nil

         for (int i = 1; i <= cols; ++i) {
            lua_pushvalue(L, 2 + i);
            lua_rawseti(L, 2, i);
         }
         lua_settop(L, 2); // pop values from first row
         lua_rawseti(L, 1, 1); // insert first row
      }

      if (row > 1) {
         lua_createtable(L, cols, 0); // 2
      } else {
         lua_pushnil(L); // 2; placeholder for row table if we have a second row
         lua_checkstack(L, cols + 2);
      }

      for (int i = 0; i < cols; ++i) {
         switch (stmt.get_type(i)) {
            case sqlite::ResultType::blob:
            case sqlite::ResultType::text:
            {
               std::size_t len;
               const char* data = static_cast<const char*>(stmt.get_blob(i, len));
               lua_pushlstring(L, data, len);
               break;
            }
            case sqlite::ResultType::integer:
               lua_pushinteger(L, stmt.get_i64(i));
               break;
            case sqlite::ResultType::real:
               lua_pushnumber(L, stmt.get_f64(i));
               break;
            default:
               lua_pushnil(L);
               break;
         }

         if (row > 1) {
            lua_rawseti(L, 2, i + 1); // put value into row table
         }
      }

      if (row > 1) {
         lua_rawseti(L, 1, row); // insert row into result table
      }
   }

   return row > 1 ? 1 : cols;
}

} // be::bglgen::()

///////////////////////////////////////////////////////////////////////////////
BglGenApp::BglGenApp(int argc, char** argv) {
   default_log().verbosity_mask(v::info_or_worse);
   default_log().clear_sinks();
   default_log().sink(ConsoleLogSink(std::cerr, std::cerr));
   try {
      using namespace cli;
      using namespace color;
      using namespace ct;
      Processor proc;

      bool show_version = false;
      bool show_help = false;
      bool verbose = false;
      S help_query;

      proc
         (prologue(Table() << header << "BENGINE GL EXTENSION LOADER GENERATOR").query())

         (synopsis(Cell() << fg_dark_gray << "[ " << fg_cyan << "OPTIONS" << fg_dark_gray << " ] " << fg_cyan << "SOURCE_DIRS"))

         (abstract("BGLgen is different from most other OpenGL extension loaders.  Instead of generating code for every function, "
            "enum, version, extension, etc. it will search through the source files of your OpenGL program, looking for symbols "
            "that look like GL calls or constants and generate bindings only for those symbols which you actually need."))

         (abstract(Cell() << "Each argument in " << fg_cyan << "SOURCE_DIRS" << fg_reset << " corresponds to a directory which will "
            "be recursively searched for source files.").verbose())

         (abstract(Cell() << "Output will always be printed to standard output.  Much of the code generation is done via internal Lua scripts.  "
            "If a file named " << fg_blue << ".bglgenrc" << reset << " exists in the working directory or a parent directory, it will be "
            "loaded after the internal scripts and before code generation begins.  This allows overriding various hooks to customize the "
            "format and behavior of the generator.").verbose())

         (param({ }, { "registry" }, "PATH", [&](const S& path) {
               registry_location_ = path;
            }).desc("Specifies the location of the OpenGL registry (gl.xml).")
            .extra(Cell() << "If not provided, BGLgen will recursively check the current directory and it parent directories "
               "until it finds a file called " << fg_blue << "gl.xml" << reset << " or the filesystem root is reached."))

         (param({ }, { "db" }, "PATH", [&](const S& path) {
               registry_db_location_ = path;
            }).desc("Specifies the location to store the registry database.")
            .extra(Cell() << "If not provided, the database file will be in the same directory as the registry, with " << fg_blue << ".db"
               << reset << " appended to the file name."))

         (flag({ "m" }, { "mem-db" }, registry_db_location_, Path(":memory:"))
            .desc("Store the GL registry into an in memory instead of on disk.")
            .extra(Cell() << "Equivalent to " << fg_yellow << "--db  " << fg_cyan << ":memory:" << reset << "."))

         (flag({ }, { "rebuild-db" }, rebuild_db_)
            .desc("Forces the registry database to be rebuilt even if its checksum matches the XML registry."))

         (param({ "x" }, { "extensions" }, "EXT", [&](const S& extensions) {
               extension_regex_ = extensions;
            })
            .desc("Specifies which types of files to consider source files when searching directories.")
            .extra(Cell() << "Default value: " << color::fg_cyan << "c|cc|cxx|cpp|hpp|inl" << color::reset)
            .extra("Only affects directories specified after it on the command line, and overrides any previous value."))

         (param({ "d" }, { "dir" }, "PATH", [&](const S& path) {
               search_dirs_.push_back(std::make_pair(Path(path), false));
            }).desc("Specifies a single directory to search non-recursively for source files."))

         (param({ "f" }, { "file" }, "PATH", [&](const S& path) {
               source_paths_.push_back(path);
            }).desc("Specifies a single source file to search for GL usages."))

         (param({ "o" }, { "output" }, "PATH", [&](const S& path) {
               output_location_ = path;
            }))

         (any([&](const S& path) {
               search_dirs_.push_back(std::make_pair(Path(path), true));
               return true;
            }))

         (param({ "p" }, { "pre-execute" }, "LUA", [&](const S& lua) {
               lua_chunks_.push_back(lua);
            }).desc("Execues the specified lua command after finding GL symbols in the input files.")
            .extra(Cell() << "The default " << fg_cyan << "process()" << reset << " function will be called "
               "afterwards unless it is overridden by an " << fg_yellow << "--execute"))

         (param({ "e" }, { "execute" }, "LUA", [&](const S& lua) {
               lua_chunks_.push_back(lua);
               do_process_ = false;
            }).desc("Replaces the default task run after finding GL symbols in the input files.")
            .extra(Cell() << "If no " << fg_yellow << "--execute" << reset << " options are specified, "
               "then the default " << fg_cyan << "process()" << reset << " function will be called."))

         (end_of_options())

         (verbosity_param({ "v" }, { "verbosity" }, "LEVEL", default_log().verbosity_mask()))

         (flag({ "V" }, { "version" }, show_version)
            .desc("Prints version information to standard output."))

         (param({ "?" }, { "help" }, "OPTION", [&](const S& value) {
               show_help = true;
               help_query = value;
            }).default_value(S())
            .allow_options_as_values(true)
            .desc(Cell() << "Outputs this help message.  For more verbose help, use " << fg_yellow << "--help")
            .extra(Cell() << nl << "If " << fg_cyan << "OPTION" << reset
               << " is provided, the options list will be filtered to show only options that contain that string."))

         (flag({ }, { "help" }, verbose).ignore_values(true))

         (exit_code(0, "There were no errors."))
         (exit_code(1, "Command line error."))
         (exit_code(2, "OpenGL registry initialization failed."))
         (exit_code(3, "Error searching for source files or GL symbols."))
         (exit_code(4, "Lua error or filesystem error while writing output file."))

         ;

      proc.process(argc, argv);

      if (show_version) {
         proc
            (prologue(BE_BGLGEN_VERSION_STRING).query())
            (prologue(BE_CORE_VERSION_STRING).query())
            (license(BE_LICENSE).query())
            (license(BE_COPYRIGHT).query())
            ;
      }

      if (show_help) {
         proc.describe(std::cout, verbose, help_query);
         status_ = 1;
      } else if (show_version) {
         proc.describe(std::cout, verbose, ids::cli_describe_section_prologue);
         proc.describe(std::cout, verbose, ids::cli_describe_section_license);
         status_ = 1;
      }

   } catch (const cli::OptionError& e) {
      status_ = 1;
      cli::log_exception(e);
   } catch (const cli::ArgumentError& e) {
      status_ = 1;
      cli::log_exception(e);
   } catch (const FatalTrace& e) {
      status_ = 1;
      log_exception(e);
   } catch (const RecoverableTrace& e) {
      status_ = 1;
      log_exception(e);
   } catch (const fs::filesystem_error& e) {
      status_ = 1;
      log_exception(e);
   } catch (const std::system_error& e) {
      status_ = 1;
      log_exception(e);
   } catch (const std::exception& e) {
      status_ = 1;
      log_exception(e);
   }
}

///////////////////////////////////////////////////////////////////////////////
int BglGenApp::operator()() {
   if (status_ != 0) {
      return status_;
   }

   try {
      context_ = belua::Context({
         belua::id_module,
         belua::logging_module,
         belua::interpolate_string_module,
         belua::time_module,
         belua::util_module,
         belua::fs_module,
         belua::fnv256_module,
         belua::blt_module,
         belua::blt_compile_module,
         belua::blt_debug_module
      });

      luaL_Reg fn[] {
         { "query", gl_registry_query },
         { nullptr, nullptr }
      };

      luaL_newlib(context_.L(), fn);
      lua_setglobal(context_.L(), "gl_registry");

      context_.execute(inflate_bglgen_core(), "@BGLgen core"); // also loads .bglrc
      get_lua_defaults_();

      std::unordered_multimap<S, SymbolUsage> symbols;

      if (!search_dirs_.empty() || !source_paths_.empty()) {
         init_extension_regex_();

         auto& fg_executor = service<SimpleInlineExecutor>(ids::bglgen_service_simple_inline_executor_foreground);
         auto& bg_executor = service<SimpleThreadPoolExecutor>(ids::bglgen_service_simple_thread_pool_executor_background);
         bg_executor.threads(std::thread::hardware_concurrency());

         for (auto& p : search_dirs_) {
            auto& dir_path = p.first;
            auto type = p.second ? util::PathMatchType::recursive_files : util::PathMatchType::files;
            bg_executor.post(
               [&,type]() {
                  find_symbols_in_dir(dir_path, extension_regex_, type, symbols, status_);
               });
         }

         for (auto& file_path : source_paths_) {
            bg_executor.post(
               [&]() {
                  find_symbols_in_file(file_path, symbols, status_);
               });
         }

         load_bgl_default_template(context_.L());
         init_registry_();

         while (!bg_executor.empty()) {
            if (!fg_executor.run()) {
               std::this_thread::yield();
            }
         }

         bg_executor.shutdown();
         fg_executor.shutdown();
         fg_executor.run();

         search_dirs_.clear();
         source_paths_.clear();

      } else {
         load_bgl_default_template(context_.L());
         init_registry_();
      }

      if (status_ != 0) {
         return status_;
      }

      std::unordered_map<S, U32> unique_functions;
      std::unordered_map<S, U32> unique_constants;

      for (auto& sym : symbols) {
         if (sym.first[0] == 'g') {
            if (unique_functions.count(sym.first) > 0) {
               unique_functions[sym.first] += sym.second.weight;
            } else {
               unique_functions[sym.first] = sym.second.weight;
            }
         } else {
            if (unique_constants.count(sym.first) > 0) {
               unique_constants[sym.first] += sym.second.weight;
            } else {
               unique_constants[sym.first] = sym.second.weight;
            }
         }
         if (sym.second.check.empty()) {
            be_short_debug("") << "Unchecked GL Symbol: " << S(sym.first) << " (" << sym.second.path.generic_string() << " : " << sym.second.line << ")" | default_log();
         } else {
            be_short_debug("") << "Checked GL Symbol: " << S(sym.first) << " (" << sym.second.path.generic_string() << " : " << sym.second.line << ")" | default_log();
         }
      }

      be_short_verbose("") << "Found " << symbols.size() << " references to " << unique_functions.size() << " functions and " << unique_constants.size() << " constants/types." | default_log();

      add_gl_data(context_.L(), "found_functions", true, unique_functions, symbols);
      unique_functions.clear();
      add_gl_data(context_.L(), "found_constants", false, unique_constants, symbols);
      unique_constants.clear();
      symbols.clear();

      sqlite::Transaction tx(db_);

      for (S& lua : lua_chunks_) {
         context_.execute(lua, "@--execute Chunk");
      }

      if (do_process_) {
         lua_getglobal(context_.L(), "process");
         belua::ecall(context_.L(), 0, 0);
      }

      lua_getglobal(context_.L(), "reset");
      belua::ecall(context_.L(), 0, 1);
      
      S result;
      if (lua_type(context_.L(), -1) == LUA_TSTRING) {
         std::size_t len;
         const char* ptr = lua_tolstring(context_.L(), -1, &len);
         result = util::normalize_newlines_copy(SV(ptr, len));
      }

      if (output_location_.empty()) {
         std::cout << result;
         std::cout.flush();
      } else {
         if (fs::exists(output_location_)) {
            S current = util::get_text_file_contents_string(output_location_);
            if (current != result) {
               util::put_text_file_contents(output_location_, result);
            }
         } else {
            util::put_text_file_contents(output_location_, result);
         }
      }

   } catch (const sqlite::SqlTrace& e) {
      status_ = 4;
      log_exception(e);
   } catch (const sqlite::SqlError& e) {
      status_ = 4;
      log_exception(e);
   } catch (const belua::LuaTrace& e) {
      status_ = 4;
      log_exception(e);
   } catch (const belua::LuaError& e) {
      status_ = 4;
      log_exception(e);
   } catch (const FatalTrace& e) {
      status_ = 4;
      log_exception(e);
   } catch (const RecoverableTrace& e) {
      status_ = 4;
      log_exception(e);
   } catch (const fs::filesystem_error& e) {
      status_ = 4;
      log_exception(e);
   } catch (const std::system_error& e) {
      status_ = 4;
      log_exception(e);
   } catch (const std::exception& e) {
      status_ = 4;
      log_exception(e);
   }

   return status_;
}

///////////////////////////////////////////////////////////////////////////////
void BglGenApp::get_lua_defaults_() {
   lua_State* L = context_.L();
   
   if (registry_location_.empty()) {
      registry_location_ = lua_get_path(L, "default_registry_path");
   }

   if (registry_db_location_.empty()) {
      registry_db_location_ = lua_get_path(L, "default_registry_db_path");
   }

   if (output_location_.empty()) {
      output_location_ = lua_get_path(L, "default_output_path");
   }

   if (search_dirs_.empty() && source_paths_.empty()) {
      if (LUA_TTABLE == lua_getglobal(L, "default_search_paths")) {
         lua_Integer n = (lua_Integer)lua_rawlen(L, -1);
         for (lua_Integer i = 1; i <= n; ++i) {
            if (LUA_TSTRING == lua_rawgeti(L, -1, i)) {
               std::size_t len;
               const char* path = lua_tolstring(L, -1, &len);
               search_dirs_.push_back(std::make_pair(Path(path, path + len), true));
            }
            lua_pop(L, 1);
         }
      }
      lua_pop(L, 1);
   }

   if (!search_dirs_.empty() && extensions_.empty() && extension_regex_.empty()) {
      auto type = lua_getglobal(L, "default_source_extensions");
      if (LUA_TTABLE == type) {
         lua_Integer n = (lua_Integer)lua_rawlen(L, -1);
         for (lua_Integer i = 1; i <= n; ++i) {
            if (LUA_TSTRING == lua_rawgeti(L, -1, i)) {
               std::size_t len;
               const char* ext = lua_tolstring(L, -1, &len);
               extensions_.push_back(S(ext, ext + len));
            }
            lua_pop(L, 1);
         }
      } else if (LUA_TSTRING == type) {
         std::size_t len;
         const char* re = lua_tolstring(L, -1, &len);
         extension_regex_.assign(re, re + len);
      }
      lua_pop(L, 1);
   }
}

///////////////////////////////////////////////////////////////////////////////
void BglGenApp::init_registry_() {
   try {
      if (registry_location_.empty()) {
         Path p = util::cwd();
         for (;;) {
            if (Path gl = p / "gl.xml"; fs::exists(gl)) {
               registry_location_ = gl;
               break;
            }

            if (p == p.root_path()) {
               break;
            }

            p = p.parent_path();
         }
      }

      if (registry_location_.empty()) {
         throw fs::filesystem_error("XML registry not found!", std::make_error_code(std::errc::no_such_file_or_directory));
      }

      if (registry_db_location_.empty()) {
         registry_db_location_ = fs::canonical(registry_location_).string() + ".db";
      }

      db_ = init_registry(registry_location_, registry_db_location_, rebuild_db_);
      cache_ = std::make_unique<sqlite::StaticStmtCache>(db_);
      cache = cache_.get();

   } catch (const sqlite::SqlTrace& e) {
      log_exception(e);
      status_ = 2;
   } catch (const sqlite::SqlError& e) {
      log_exception(e);
      status_ = 2;
   } catch (const fs::filesystem_error& e) {
      log_exception(e);
      status_ = 2;
   } catch (const std::system_error& e) {
      log_exception(e);
      status_ = 2;
   } catch (const std::exception& e) {
      log_exception(e);
      status_ = 2;
   }
}

///////////////////////////////////////////////////////////////////////////////
void BglGenApp::init_extension_regex_() {
   if (!extensions_.empty()) {
      for (const S& ext : extensions_) {
         if (ext.empty()) {
            continue;
         }

         if (!extension_regex_.empty()) {
            extension_regex_.append(1, '|');
         }

         extension_regex_.append(ext);
      }

      extensions_.clear();
   }

   if (extension_regex_.empty()) {
      extension_regex_ = "c|cc|cxx|cpp|hpp|inl";
   }
}

} // be::bglgen
