#include "bglgen_app.hpp"
#include "lexer.hpp"
#include "bglgen_lua.hpp"
#include "version.hpp"
#include "gl_registry.hpp"
#include <be/core/version.hpp>
#include <be/cli/cli.hpp>
#include <be/core/logging.hpp>
#include <be/core/console_log_sink.hpp>
#include <be/core/log_exception.hpp>
#include <be/util/path_glob.hpp>
#include <be/core/alg.hpp>
#include <be/belua/context.hpp>
#include <be/belua/lua_helpers.hpp>
#include <be/core/lua_modules.hpp>
#include <be/blt/lua_modules.hpp>
#include <be/util/lua_modules.hpp>
#include <be/sqlite/static_stmt_cache.hpp>
#include <be/sqlite/log_exception.hpp>
#include <be/util/get_file_contents.hpp>

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
sqlite::Db* registry = nullptr;
sqlite::StaticStmtCache* cache = nullptr;

/////////////////////////////////////////////////////////////////////////////////
//int gl_registry_list_features(lua_State* L) {
//   // get all feature names corresponding to a particular api
//   const char* api_name = luaL_checkstring(L, 1);
//   lua_settop(L, 1);
//   lua_newtable(L); // 2
//
//   int index = 1;
//   for (pugi::xml_node feature : registry.children("feature")) {
//      if (0 == strcmp(api_name, feature.attribute("api").value())) {
//         lua_pushstring(L, feature.attribute("name").value());
//         lua_rawseti(L, 2, index);
//         ++index;
//      }
//   }
//
//   return 1;
//}
//
/////////////////////////////////////////////////////////////////////////////////
//int gl_registry_list_extensions(lua_State* L) {
//   // get all extension names corresponding to a particular api
//   const char* api_name = luaL_checkstring(L, 1);
//   lua_settop(L, 1);
//   lua_newtable(L); // 2
//
//   int index = 1;
//   for (pugi::xml_node extensions : registry.children("extensions")) {
//      for (pugi::xml_node extension : extensions.children("extension")) {
//         std::regex support_re(extension.attribute("supported").value());
//         if (std::regex_match(api_name, support_re)) {
//            lua_pushstring(L, extension.attribute("name").value());
//            lua_rawseti(L, 2, index);
//            ++index;
//         }
//      }
//   }
//
//   return 1;
//}
//
/////////////////////////////////////////////////////////////////////////////////
//int gl_registry_get_feature(lua_State* L) {
//   // get information about a specific feature.
//   const char* feature_name = luaL_checkstring(L, 1);
//
//   S query_api;
//   if (lua_gettop(L) > 1) {
//      query_api = luaL_checkstring(L, 2);
//   }
//
//   lua_settop(L, 1);
//
//   for (pugi::xml_node feature : registry.children("feature")) {
//      if (0 == strcmp(feature_name, feature.attribute("name").value())) {
//         if (!query_api.empty() && 0 != strcmp(query_api.c_str(), feature.attribute("api").value())) {
//            return 0;
//         }
//
//         /*
//         {
//            name = "GL_VERSION_1_1",
//            number = "1.1",
//            protect = "asdf",
//            contents = {
//               gl = {
//                  _ = {
//                     enums = {
//                        GL_STENCIL_INDEX = true,
//                        ...
//                     },
//                     commands = {
//                        glGenerateMipmap = true,
//                        ...
//                     }
//                  }
//               }
//            }
//         }
//         */
//
//         lua_createtable(L, 0, 4); // 2
//
//         lua_pushliteral(L, "name"); // 3
//         lua_pushvalue(L, 1); // 4
//         lua_rawset(L, 2);
//
//         lua_pushliteral(L, "number"); // 3
//         lua_pushstring(L, feature.attribute("number").value()); // 4
//         lua_rawset(L, 2);
//
//         if (pugi::xml_attribute protect = feature.attribute("protect"); !protect.empty()) {
//            lua_pushliteral(L, "protect"); // 3
//            lua_pushstring(L, protect.value()); // 4
//            lua_rawset(L, 2);
//         }
//
//         lua_pushliteral(L, "contents"); // 3
//         lua_createtable(L, 0, 1); // 4
//
//         lua_pushstring(L, feature.attribute("api").value()); // 5
//         lua_newtable(L); // 6
//
//         for (pugi::xml_node set : feature.children()) {
//            bool remove;
//            if (0 == strcmp(set.name(), "require")) {
//               remove = false;
//            } else if (0 == strcmp(set.name(), "remove")) {
//               remove = true;
//            } else {
//               continue;
//            }
//
//            const char* profile = set.attribute("profile").value();
//            if (0 == strcmp(profile, "")) profile = "_";
//
//            lua_pushstring(L, profile); // 7
//            if (LUA_TNIL == lua_rawget(L, 6)) { // 7
//               lua_pop(L, 1);
//               lua_createtable(L, 0, 3); // 7
//               lua_pushstring(L, profile); // 8
//               lua_pushvalue(L, 7); // 9
//               lua_rawset(L, 6);
//            }
//
//            for (pugi::xml_node item : set.children()) {
//               const char* which;
//               if (0 == strcmp(item.name(), "enum")) {
//                  which = "enums";
//               } else if (0 == strcmp(item.name(), "command")) {
//                  which = "commands";
//               } else if (0 == strcmp(item.name(), "type")) {
//                  which = "types";
//               } else {
//                  continue;
//               }
//
//               lua_pushstring(L, which); // 8
//               if (LUA_TNIL == lua_rawget(L, 7)) { // 8
//                  lua_pop(L, 1);
//                  lua_newtable(L); // 8
//                  lua_pushstring(L, which); // 9
//                  lua_pushvalue(L, 8); // 10
//                  lua_rawset(L, 7);
//               }
//
//               lua_pushstring(L, item.attribute("name").value()); // 9
//               lua_pushboolean(L, remove ? 0 : 1); // 10
//               lua_rawset(L, 8);
//
//               lua_pop(L, 1);
//            }
//
//            lua_pop(L, 1);
//         }
//
//         lua_rawset(L, 4);
//         lua_rawset(L, 2);
//         return 1;
//      }
//   }
//   return 0;
//}
//
/////////////////////////////////////////////////////////////////////////////////
//int gl_registry_get_extension(lua_State* L) {
//   // get information about a specific extension.
//   const char* extension_name = luaL_checkstring(L, 1);
//
//   S query_api;
//   if (lua_gettop(L) > 1) {
//      query_api = luaL_checkstring(L, 2);
//   }
//
//   lua_settop(L, 1);
//
//   for (pugi::xml_node extensions : registry.children("extensions")) {
//      for (pugi::xml_node extension : extensions.children("extension")) {
//         if (0 == strcmp(extension_name, extension.attribute("name").value())) {
//            S supported = extension.attribute("supported").value();
//            if (!query_api.empty()) {
//               std::regex supported_re(supported);
//               if (!std::regex_match(query_api, supported_re)) {
//                  return 0;
//               }
//            }
//
//            /*
//            {
//               name = "GL_KHR_debug",
//               protect = "asdf",
//               contents = {
//                  gl = {
//                     _ = {
//                        enums = {
//                           GL_STENCIL_INDEX = true,
//                           ...
//                        },
//                        commands = {
//                           glGenerateMipmap = true,
//                           ...
//                        }
//                     }
//                  },
//                  gles = {
//                     _ = {
//                        enums = {
//                           GL_STENCIL_INDEX = true,
//                           ...
//                        }
//                     }
//                  }
//               }
//            }
//            */
//
//            lua_createtable(L, 0, 3); // 2
//
//            lua_pushliteral(L, "name"); // 3
//            lua_pushvalue(L, 1); // 4
//            lua_rawset(L, 2);
//
//            if (pugi::xml_attribute protect = extension.attribute("protect"); !protect.empty()) {
//               lua_pushliteral(L, "protect"); // 3
//               lua_pushstring(L, protect.value()); // 4
//               lua_rawset(L, 2);
//            }
//
//            lua_pushliteral(L, "contents"); // 3
//            lua_createtable(L, 0, 1); // 4
//
//            std::vector<S> apis;
//            if (!query_api.empty()) {
//               apis.push_back(query_api);
//            } else {
//               std::size_t start = 0, end = 0;
//               while ((end = supported.find('|', start)) != S::npos) {
//                  apis.push_back(supported.substr(start, end - start));
//                  start = end + 1;
//               }
//               apis.push_back(supported.substr(start));
//            }
//
//            for (S& api : apis) {
//               lua_pushstring(L, api.c_str()); // 5
//               lua_newtable(L); // 6
//               lua_rawset(L, 4);
//            }
//            
//            for (pugi::xml_node set : extension.children()) {
//               bool remove;
//               if (0 == strcmp(set.name(), "require")) {
//                  remove = false;
//               } else if (0 == strcmp(set.name(), "remove")) {
//                  remove = true;
//               } else {
//                  continue;
//               }
//
//               const char* api = set.attribute("api").value();
//               const char* profile = set.attribute("profile").value();
//               if (0 == strcmp(profile, "")) profile = "_";
//
//               if (0 == strcmp(api, "")) {
//                  lua_pushstring(L, api); // 5
//                  if (LUA_TNIL != lua_rawget(L, 4)) { // 5
//                     lua_pushstring(L, profile); // 6
//                     if (LUA_TNIL == lua_rawget(L, 5)) { // 6
//                        lua_pop(L, 1);
//                        lua_newtable(L); // 6
//                        lua_pushstring(L, profile); // 7
//                        lua_pushvalue(L, 6); // 8
//                        lua_rawset(L, 5);
//                     }
//
//                     for (pugi::xml_node item : set.children()) {
//                        const char* which;
//                        if (0 == strcmp(item.name(), "enum")) {
//                           which = "enums";
//                        } else if (0 == strcmp(item.name(), "command")) {
//                           which = "commands";
//                        } else if (0 == strcmp(item.name(), "type")) {
//                           which = "types";
//                        } else {
//                           continue;
//                        }
//
//                        lua_pushstring(L, which); // 7
//                        if (LUA_TNIL == lua_rawget(L, 6)) { // 7
//                           lua_pop(L, 1);
//                           lua_newtable(L); // 7
//                           lua_pushstring(L, which); // 8
//                           lua_pushvalue(L, 7); // 9
//                           lua_rawset(L, 6);
//                        }
//
//                        lua_pushstring(L, item.attribute("name").value()); // 8
//                        lua_pushboolean(L, remove ? 0 : 1); // 9
//                        lua_rawset(L, 7);
//
//                        lua_pop(L, 1);
//                     }
//                     lua_pop(L, 1);
//                  }
//                  lua_pop(L, 1);
//               } else {
//                  for (S& a : apis) {
//                     lua_pushstring(L, a.c_str()); // 5
//                     if (LUA_TNIL != lua_rawget(L, 4)) { // 5
//
//                        lua_pushstring(L, profile); // 6
//                        if (LUA_TNIL == lua_rawget(L, 5)) { // 6
//                           lua_pop(L, 1);
//                           lua_newtable(L); // 6
//                           lua_pushstring(L, profile); // 7
//                           lua_pushvalue(L, 6); // 8
//                           lua_rawset(L, 5);
//                        }
//
//                        for (pugi::xml_node item : set.children()) {
//                           const char* which;
//                           if (0 == strcmp(item.name(), "enum")) {
//                              which = "enums";
//                           } else if (0 == strcmp(item.name(), "command")) {
//                              which = "commands";
//                           } else if (0 == strcmp(item.name(), "type")) {
//                              which = "types";
//                           } else {
//                              continue;
//                           }
//
//                           lua_pushstring(L, which); // 7
//                           if (LUA_TNIL == lua_rawget(L, 6)) { // 7
//                              lua_pop(L, 1);
//                              lua_newtable(L); // 7
//                              lua_pushstring(L, which); // 8
//                              lua_pushvalue(L, 7); // 9
//                              lua_rawset(L, 6);
//                           }
//
//                           lua_pushstring(L, item.attribute("name").value()); // 8
//                           lua_pushboolean(L, remove ? 0 : 1); // 9
//                           lua_rawset(L, 7);
//
//                           lua_pop(L, 1);
//                        }
//
//                        lua_pop(L, 1);
//                     }
//                     lua_pop(L, 1);
//                  }
//               }
//            }
//            lua_rawset(L, 2);
//            return 1;
//         }
//      }
//   }
//   return 0;
//}
//
/////////////////////////////////////////////////////////////////////////////////
//int gl_registry_get_command(lua_State* L) {
//   // get information about a specific command.
//   const char* command_name = luaL_checkstring(L, 1);
//   lua_settop(L, 1);
//
//   for (pugi::xml_node commands : registry.children("commands")) {
//      for (pugi::xml_node command : commands.children("command")) {
//         auto proto = command.child("proto");
//         auto name = proto.child("name");
//
//         if (0 == strcmp(command_name, name.child_value())) {
//            lua_createtable(L, 0, 5); // 2
//
//            lua_pushliteral(L, "name"); // 3
//            lua_pushstring(L, command_name); // 4
//            lua_rawset(L, 2);
//
//            lua_pushliteral(L, "return"); // 3
//            lua_createtable(L, 0, 3); // 4
//            {
//               if (auto ptype = proto.child("ptype"); !ptype.empty()) {
//                  lua_pushliteral(L, "type"); // 5
//                  lua_pushstring(L, ptype.child_value()); // 6
//                  lua_rawset(L, 4);
//               }
//
//               if (auto group = proto.attribute("group"); !group.empty()) {
//                  lua_pushliteral(L, "group"); // 5
//                  lua_pushstring(L, group.value()); // 6
//                  lua_rawset(L, 4);
//               }
//
//               S return_decl;
//               for (auto node : proto.children()) {
//                  if (node.type() != pugi::node_element) {
//                     return_decl.append(node.value());
//                  } else if (0 == strcmp(node.name(), "name")) {
//                     break;
//                  } else {
//                     return_decl.append(node.child_value());
//                  }
//               }
//
//               lua_pushliteral(L, "declaration"); // 5
//               lua_pushlstring(L, return_decl.c_str(), ( int ) return_decl.size()); // 6
//               lua_rawset(L, 4);
//            }
//            lua_rawset(L, 2);
//
//            lua_pushliteral(L, "params"); // 3
//            lua_newtable(L); // 4
//
//            I32 param_index = 1;
//            for (auto node : command.children()) {
//               if (node.type() == pugi::node_element) {
//                  if (0 == strcmp(node.name(), "param")) {
//                     lua_newtable(L); // 5
//
//                     lua_pushliteral(L, "name"); // 6
//                     lua_pushstring(L, node.child("name").child_value()); // 7
//                     lua_rawset(L, 5);
//
//                     if (auto ptype = node.child("ptype"); !ptype.empty()) {
//                        lua_pushliteral(L, "type"); // 6
//                        lua_pushstring(L, ptype.child_value()); // 7
//                        lua_rawset(L, 5);
//                     }
//
//                     if (auto group = node.attribute("group"); !group.empty()) {
//                        lua_pushliteral(L, "group"); // 6
//                        lua_pushstring(L, group.value()); // 7
//                        lua_rawset(L, 5);
//                     }
//
//                     if (auto len = node.attribute("len"); !len.empty()) {
//                        const char* val = len.value();
//
//                        lua_pushliteral(L, "raw_length"); // 6
//                        lua_pushstring(L, val); // 7
//                        lua_rawset(L, 5);
//                        
//                        U32 length = ( U32 ) strtoul(val, nullptr, 10);
//                        if (length > 0) {
//                           lua_pushliteral(L, "length"); // 6
//                           lua_pushinteger(L, length); // 7
//                           lua_rawset(L, 5);
//                        }
//                     }
//
//                     S decl;
//                     for (auto param_node : node.children()) {
//                        if (param_node.type() != pugi::node_element) {
//                           decl.append(param_node.value());
//                        } else if (0 == strcmp(param_node.name(), "name")) {
//                           break;
//                        } else {
//                           decl.append(param_node.child_value());
//                        }
//                     }
//
//                     lua_pushliteral(L, "declaration"); // 6
//                     lua_pushlstring(L, decl.c_str(), (int)decl.size()); // 7
//                     lua_rawset(L, 5);
//
//                     lua_rawseti(L, 4, param_index);
//                     ++param_index;
//                  } else if (0 == strcmp(node.name(), "alias")) {
//                     lua_pushliteral(L, "alias"); // 5
//                     lua_pushstring(L, node.attribute("name").value()); // 6
//                     lua_rawset(L, 2);
//                  } else if (0 == strcmp(node.name(), "vecequiv")) {
//                     lua_pushliteral(L, "vector_equivalent"); // 5
//                     lua_pushstring(L, node.attribute("name").value()); // 6
//                     lua_rawset(L, 2);
//                  }
//               }
//            }
//
//            lua_rawset(L, 2);
//            return 1;
//         }
//      }
//   }
//
//   return 0;
//}
//
/////////////////////////////////////////////////////////////////////////////////
//int gl_registry_get_enum(lua_State* L) {
//   // get information about a specific enum.
//   const char* enum_name = luaL_checkstring(L, 1);
//
//   S query_api;
//   if (lua_gettop(L) > 1) {
//      query_api = luaL_checkstring(L, 2);
//   }
//
//   lua_settop(L, 1);
//
//   for (pugi::xml_node enums : registry.children("enums")) {
//      for (pugi::xml_node enum_elem : enums.children("enum")) {
//         if (0 == strcmp(enum_name, enum_elem.attribute("name").value())) {
//            if (pugi::xml_attribute api = enum_elem.attribute("api"); !query_api.empty() && !api.empty()) {
//               if (0 != strcmp(api.value(), query_api.c_str())) {
//                  continue;
//               }
//            }
//
//            lua_createtable(L, 0, 9); // 2
//
//            lua_pushliteral(L, "name"); // 3
//            lua_pushstring(L, enum_name); // 4
//            lua_rawset(L, 2);
//
//            lua_pushliteral(L, "raw_value"); // 3
//            lua_pushstring(L, enum_elem.attribute("value").value()); // 4
//            lua_rawset(L, 2);
//
//            lua_pushliteral(L, "value"); // 3
//            lua_pushinteger(L, enum_elem.attribute("value").as_uint()); // 4
//            lua_rawset(L, 2);
//
//            if (auto attr = enum_elem.attribute("type"); !attr.empty()) {
//               lua_pushliteral(L, "suffix"); // 3
//               lua_pushstring(L, attr.value()); // 4
//               lua_rawset(L, 2);
//            }
//
//            if (auto attr = enum_elem.attribute("alias"); !attr.empty()) {
//               lua_pushliteral(L, "alias"); // 3
//               lua_pushstring(L, attr.value()); // 4
//               lua_rawset(L, 2);
//            }
//
//            if (auto attr = enum_elem.attribute("api"); !attr.empty()) {
//               lua_pushliteral(L, "api"); // 3
//               lua_pushstring(L, attr.value()); // 4
//               lua_rawset(L, 2);
//            }
//
//            lua_pushliteral(L, "namespace"); // 3
//            lua_pushstring(L, enums.attribute("namespace").value()); // 4
//            lua_rawset(L, 2);
//
//            if (0 == strcmp("bitmask", enums.attribute("type").value())) {
//               lua_pushliteral(L, "bitmask"); // 3
//               lua_pushboolean(L, 1); // 4
//               lua_rawset(L, 2);
//            }
//            
//            if (auto attr = enums.attribute("vendor"); !attr.empty()) {
//               lua_pushliteral(L, "vendor"); // 3
//               lua_pushstring(L, attr.value()); // 4
//               lua_rawset(L, 2);
//            }
//
//            return 1;
//         }
//      }
//   }
//
//   return 0;
//}
//
/////////////////////////////////////////////////////////////////////////////////
//int gl_registry_get_type(lua_State* L) {
//   // get information about a specific type.
//   const char* type_name = luaL_checkstring(L, 1);
//
//   S query_api;
//   if (lua_gettop(L) > 1) {
//      query_api = luaL_checkstring(L, 2);
//   }
//
//   S apientry;
//   if (lua_gettop(L) > 2) {
//      apientry = luaL_checkstring(L, 3);
//   } else {
//      apientry = "APIENTRY";
//   }
//
//   lua_settop(L, 1);
//
//   for (pugi::xml_node types : registry.children("types")) {
//      for (pugi::xml_node type : types.children("type")) {
//         if (pugi::xml_attribute api = type.attribute("api"); !query_api.empty() && !api.empty()) {
//            if (0 != strcmp(api.value(), query_api.c_str())) {
//               continue;
//            }
//         }
//
//         const char* name;
//         if (auto name_element = type.child("name"); !name_element.empty()) {
//            name = name_element.child_value();
//         } else if (auto name_attrib = type.attribute("name"); !name_attrib.empty()) {
//            name = name_attrib.value();
//         } else {
//            continue;
//         }
//         if (0 == strcmp(name, type_name)) {
//            lua_createtable(L, 0, 4); // 2
//
//            lua_pushliteral(L, "name"); // 3
//            lua_pushstring(L, name); // 4
//            lua_rawset(L, 2);
//
//            if (pugi::xml_attribute api = type.attribute("api"); !api.empty()) {
//               lua_pushliteral(L, "api"); // 3
//               lua_pushstring(L, api.value()); // 4
//               lua_rawset(L, 2);
//            }
//
//            if (pugi::xml_attribute requires = type.attribute("requires"); !requires.empty()) {
//               lua_pushliteral(L, "requires"); // 3
//               lua_pushstring(L, requires.value()); // 4
//               lua_rawset(L, 2);
//            }
//            
//            S contents;
//            for (pugi::xml_node child : type.children()) {
//               if (child.type() == pugi::node_element) {
//                  if (0 == strcmp(child.name(), "apientry")) {
//                     contents += apientry;
//                  } else {
//                     contents += child.child_value();
//                  }
//               } else {
//                  contents += child.value();
//               }
//            }
//
//            lua_pushliteral(L, "declaration");
//            lua_pushlstring(L, contents.c_str(), contents.size());
//            lua_rawset(L, 2);
//
//            return 1;
//         }
//      }
//   }
//
//   return 0;
//}
//
/////////////////////////////////////////////////////////////////////////////////
//int gl_registry_get_group(lua_State* L) {
//   // get information about a specific enum group.
//   const char* group_name = luaL_checkstring(L, 1);
//   lua_settop(L, 1);
//
//   for (pugi::xml_node groups : registry.children("groups")) {
//      for (pugi::xml_node group : groups.children("group")) {
//         if (0 == strcmp(group_name, group.attribute("name").value())) {
//            lua_createtable(L, 0, 2); // 2
//
//            lua_pushliteral(L, "name"); // 3
//            lua_pushstring(L, group_name); // 4
//            lua_rawset(L, 2);
//
//            lua_pushliteral(L, "enums"); // 3
//            lua_newtable(L); // 4
//
//            for (pugi::xml_node enum_elem : group.children("enum")) {
//               lua_pushstring(L, enum_elem.attribute("name").value());
//               lua_pushboolean(L, 1);
//               lua_rawset(L, 4);
//            }
//
//            lua_rawset(L, 2);
//
//            return 1;
//         }
//      }
//   }
//
//   return 0;
//}

} // be::bglgen::()

///////////////////////////////////////////////////////////////////////////////
BglGenApp::BglGenApp(int argc, char** argv) {
   default_log().verbosity_mask(v::info_or_worse);
   default_log().clear_sinks();
   default_log().sink(ConsoleLogSink(std::clog, std::cerr));
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

      (param({ "e" }, { "execute" }, "LUA", [&](const S& lua) {
            lua_chunks_.push_back(lua);
         }).desc("Executes the specified lua command after finding GL symbols in the input files.")
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
      } else if (show_version) {
         proc.describe(std::cout, verbose, ids::cli_describe_section_prologue);
         proc.describe(std::cout, verbose, ids::cli_describe_section_license);
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

   registry = &db;
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
         //{ "list_features", gl_registry_list_features },
         //{ "list_extensions", gl_registry_list_extensions },
         //{ "get_feature", gl_registry_get_feature },
         //{ "get_extension", gl_registry_get_extension },
         //{ "get_command", gl_registry_get_command },
         //{ "get_enum", gl_registry_get_enum },
         //{ "get_type", gl_registry_get_type },
         //{ "get_group", gl_registry_get_group },
         { nullptr, nullptr }
      };

      lua_rawgeti(context.L(), LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
      lua_pushliteral(context.L(), "gl_registry");
      luaL_newlib(context.L(), fn);
      lua_rawset(context.L(), -3);

      add_gl_data(context.L(), "found_functions", true, unique_functions, symbols);
      unique_functions.clear(); // don't need this anymore; clear up some memory
      add_gl_data(context.L(), "found_constants", false, unique_constants, symbols);
      unique_constants.clear(); // don't need this anymore; clear up some memory
      symbols.clear(); // don't need this anymore; clear up some memory

      context.execute(inflate_bglgen_core(), "@BGLgen core");

      if (lua_chunks_.empty()) {
         lua_getglobal(context.L(), "process");
         belua::ecall(context.L(), 0, 0);
      } else {
         for (S& lua : lua_chunks_) {
            context.execute(lua, "@--execute Chunk");
         }
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
