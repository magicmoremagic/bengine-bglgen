#include "bglgen_app.hpp"
#include "lexer.hpp"
#include "bglgen_lua.hpp"
#include "bglgen_blt.hpp"
#include "version.hpp"
#include "gl_registry.hpp"
#include <be/core/version.hpp>
#include <be/core/alg.hpp>
#include <be/core/logging.hpp>
#include <be/core/log_exception.hpp>
#include <be/core/console_log_sink.hpp>
#include <be/core/lua_modules.hpp>
#include <be/util/path_glob.hpp>
#include <be/util/get_file_contents.hpp>
#include <be/util/lua_modules.hpp>
#include <be/cli/cli.hpp>
#include <be/blt/lua_modules.hpp>
#include <be/belua/context.hpp>
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

   /* We're trying to make a table like this:
      {
         {
            name = 'glXXXXX',
            weight = 100,
            used_unchecked = true,
            refs = {
               {
                  path = '...',
                  weight = 100,
                  line = 1337,
                  checked = true
               }
            },
            { ... },
            ...
         }
      }
   */

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

      S ext_regex = "c|cc|cxx|cpp|hpp|inl";

      proc
      (prologue(Table() << header << "BENGINE GL EXTENSION LOADER GENERATOR").query())

      (synopsis(Cell() << fg_dark_gray << "[ " << fg_cyan << "OPTIONS"
                       << fg_dark_gray << " ] " << fg_cyan << "SOURCE_DIRS"))

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
         }).desc("Specifies the location to store the registry database.")
            .extra(Cell() << "If not provided, the database file will be in the same directory as the registry, with " << fg_blue << ".db"
                          << reset << " appended to the file name."))

      (flag({ }, { "rebuild-db" }, rebuild_db_)
         .desc("Forces the registry database to be rebuilt even if its checksum matches the XML registry."))

      (param({ "x" }, { "extensions" }, "EXT", ext_regex)
         .desc("Specifies which types of files to consider source files when searching directories.")
         .extra("Only affects directories specified after it on the command line, and overrides any previous value."))

      (param({ "d" }, { "dir" }, "PATH", [&](const S& path) {
            Path base = fs::absolute(path);
            if (fs::exists(base)) {
               std::vector<Path> paths = util::glob("*.*", base, util::PathMatchType::files);
               std::regex ext_re(ext_regex);
               for (Path& p : paths) {
                  if (std::regex_match(p.extension().string().substr(1), ext_re)) {
                     source_paths_.push_back(p);
                  }
               }
            } else {
               throw fs::filesystem_error("Path does not exist!", std::make_error_code(std::errc::not_a_directory));
            }
         }).desc("Specifies a single directory to search non-recursively for source files."))

      (param({ "f" }, { "file" }, "PATH", [&](const S& path) {
            source_paths_.push_back(path);
         }).desc("Specifies a single source file to search for GL usages."))

      (any([&](const S& path) {
            Path base = fs::absolute(path);
            if (fs::exists(base)) {
               std::vector<Path> paths = util::glob("*.*", base, util::PathMatchType::recursive_files_and_dirs);
               std::regex ext_re(ext_regex);
               for (Path& p : paths) {
                  if (std::regex_match(p.extension().string().substr(1), ext_re)) {
                     source_paths_.push_back(p);
                  }
               }
            } else {
               throw fs::filesystem_error("Path does not exist!", std::make_error_code(std::errc::not_a_directory));
            }
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

      (flag({ "V" }, { "version" }, show_version).desc("Prints version information to standard output."))

      (param({ "?" }, { "help" }, "OPTION", [&](const S& value) {
            show_help = true;
            help_query = value;
         }).default_value(S())
            .allow_options_as_values(true)
            .desc(Cell() << "Outputs this help message.  For more verbose help, use " << fg_yellow << "--help")
            .extra(Cell() << nl << "If " << fg_cyan << "OPTION" << reset
                          << " is provided, the options list will be filtered to show only options that contain that string."))

      (flag({ }, { "help" }, verbose).ignore_values(true))

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
         status_ = 2;
      } else if (show_version) {
         proc.describe(std::cout, verbose, ids::cli_describe_section_prologue);
         proc.describe(std::cout, verbose, ids::cli_describe_section_license);
         status_ = 2;
      }

   } catch (const cli::OptionError& e) {
      status_ = 2;
      cli::log_exception(e);
   } catch (const cli::ArgumentError& e) {
      status_ = 2;
      cli::log_exception(e);
   } catch (const FatalTrace& e) {
      status_ = 2;
      log_exception(e);
   } catch (const RecoverableTrace& e) {
      status_ = 2;
      log_exception(e);
   } catch (const fs::filesystem_error& e) {
      status_ = 2;
      log_exception(e);
   } catch (const std::system_error& e) {
      status_ = 2;
      log_exception(e);
   } catch (const std::exception& e) {
      status_ = 2;
      log_exception(e);
   }
}

///////////////////////////////////////////////////////////////////////////////
int BglGenApp::operator()() {
   if (status_ != 0) {
      return status_;
   }

   sqlite::Db db;

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

      db = init_registry(registry_location_, registry_db_location_, rebuild_db_);

   } catch (const sqlite::SqlTrace& e) {
      log_exception(e);
      return 1;
   } catch (const sqlite::SqlError& e) {
      log_exception(e);
      return 1;
   } catch (const fs::filesystem_error& e) {
      log_exception(e);
      return 1;
   } catch (const std::system_error& e) {
      log_exception(e);
      return 1;
   } catch (const std::exception& e) {
      log_exception(e);
      return 1;
   }

   sqlite::StaticStmtCache db_cache(db);
   cache = &db_cache;

   std::unordered_multimap<S, SymbolUsage> symbols;
   for (Path& p : source_paths_) {
      be_short_debug("") << "Source Path: " << p.generic_string() | default_log();
      try {
         S data = util::get_file_contents_string(p);
         Lexer lex(p, data, symbols);
         lex();

      } catch (const fs::filesystem_error& e) {
         log_exception(e);
      } catch (const std::system_error& e) {
         log_exception(e);
      } catch (const std::exception& e) {
         log_exception(e);
      }
   }

   source_paths_.clear(); // don't need this anymore; clear up some memory

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

   try {
      sqlite::Transaction tx(db);

      belua::Context context({
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

      luaL_newlib(context.L(), fn);
      lua_setglobal(context.L(), "gl_registry");

      add_gl_data(context.L(), "found_functions", true, unique_functions, symbols);
      unique_functions.clear(); // don't need this anymore; clear up some memory
      add_gl_data(context.L(), "found_constants", false, unique_constants, symbols);
      unique_constants.clear(); // don't need this anymore; clear up some memory
      symbols.clear(); // don't need this anymore; clear up some memory

      context.execute(inflate_bglgen_core(), "@BGLgen core");

      {
         S bgl_default_template = inflate_bgl_default_template();
         lua_getglobal(context.L(), "register_template_string");
         lua_pushlstring(context.L(), bgl_default_template.c_str(), ( int ) bgl_default_template.size());
         lua_pushstring(context.L(), "bgl_default");
         belua::ecall(context.L(), 2, 0);
      }

      for (S& lua : lua_chunks_) {
         context.execute(lua, "@--execute Chunk");
      }

      if (do_process_) {
         lua_getglobal(context.L(), "process");
         belua::ecall(context.L(), 0, 0);
      }

      S result;
      lua_getglobal(context.L(), "reset");
      belua::ecall(context.L(), 0, 1);
      
      if (lua_type(context.L(), -1) == LUA_TSTRING) {
         std::size_t len;
         const char* ptr = lua_tolstring(context.L(), -1, &len);
         if (ptr) {
            result.assign(ptr, len);
         }
      }

      std::cout << result;
      std::cout.flush();

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

} // be::bglgen
