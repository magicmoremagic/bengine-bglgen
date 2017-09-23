#include "gl_registry.hpp"
#include "sql_ids.hpp"
#include <be/core/id.hpp>
#include <be/core/logging.hpp>
#include <be/util/get_file_contents.hpp>
#include <be/util/keyword_parser.hpp>
#include <be/sqlite/static_stmt_cache.hpp>
#include <be/sqlite/transaction.hpp>
#include <be/sqlite/queries.hpp>
#include <pugixml/pugixml.hpp>
#include <map>
#include <set>

namespace be::bglgen {
namespace {

Id get_schema_checksum();

//////////////////////////////////////////////////////////////////////////////
void create_registry_db(sqlite::Db& db) {
   be_short_info("") << "Building registry DB..." | default_log();

   sqlite::Transaction tx(db);

   /*!!
   tables = {
      'comments', 'apis', 'profiles',
      'types', 'type_declarations', 'type_dependencies',
      'groups', 'group_enums',
      'enums', 'enum_declarations', 'enum_aliases',
      'commands', 'command_signature', 'command_aliases',
      'components', 'component_apis', 'component_types', 'component_enums', 'component_commands'
   }
   indices = {
      'enum_aliases_reverse', 'enum_group',
      'command_aliases_reverse',
      'api_component', 'type_component', 'enum_component', 'command_component'
   }
   views = {
      'features', 'extensions',
      'type_components', 'enum_components', 'command_components',
      'api_types', 'api_enums', 'api_group_enums'
   }

   schema_ids = { 'CT_REGISTRY_INFO' }
   table_ids = { 'CT_REGISTRY_INFO' }
   index_ids = { }
   view_ids = { }
   for i = 1, #tables do
      local id = 'CT_' .. string.upper(tables[i])
      schema_ids[#schema_ids+1] = id
      table_ids[#table_ids+1] = id
   end
   for i = 1, #indices do
      local id = 'CI_' .. string.upper(indices[i])
      schema_ids[#schema_ids+1] = id
      index_ids[#index_ids+1] = id
   end
   for i = 1, #views do
      local id = 'CV_' .. string.upper(views[i])
      schema_ids[#schema_ids+1] = id
      view_ids[#view_ids+1] = id
   end
   
   register_template_string([[`with each $ using # { nl `sqlite::exec(db, BEIDN_BGLGEN_SQL_`$`);`} nl]], 'exec_ids')
   write_template('exec_ids', schema_ids) !! 40 */
   /* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */

   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_REGISTRY_INFO);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_COMMENTS);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_APIS);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_PROFILES);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_TYPES);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_TYPE_DECLARATIONS);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_TYPE_DEPENDENCIES);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_GROUPS);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_GROUP_ENUMS);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_ENUMS);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_ENUM_DECLARATIONS);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_ENUM_ALIASES);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_COMMANDS);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_COMMAND_SIGNATURE);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_COMMAND_ALIASES);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_COMPONENTS);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_COMPONENT_APIS);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_COMPONENT_TYPES);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_COMPONENT_ENUMS);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CT_COMPONENT_COMMANDS);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CI_ENUM_ALIASES_REVERSE);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CI_ENUM_GROUP);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CI_COMMAND_ALIASES_REVERSE);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CI_API_COMPONENT);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CI_TYPE_COMPONENT);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CI_ENUM_COMPONENT);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CI_COMMAND_COMPONENT);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CV_FEATURES);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CV_EXTENSIONS);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CV_TYPE_COMPONENTS);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CV_ENUM_COMPONENTS);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CV_COMMAND_COMPONENTS);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CV_API_TYPES);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CV_API_ENUMS);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CV_API_GROUP_ENUMS);

   /* ######################### END OF GENERATED CODE ######################### */

   sqlite::Stmt s(db, "INSERT INTO registry_info (schema_checksum, xml_checksum, xml_size, xml_path) VALUES (?, ?, ?, ?)");
   s.bind(1, ( U64 ) get_schema_checksum());
   s.bind(2, 0);
   s.bind(3, 0);
   s.bind(4, "");
   s.step();

   tx.commit();
}

//////////////////////////////////////////////////////////////////////////////
Id get_schema_checksum() {
   /*!! register_template_string([[static const Id checksum = Id(`with each $ using # { nl `   BEIDN_BGLGEN_SQL_` $ } nl `);]], 'schema_checksum')
   write_template('schema_checksum', schema_ids) !! 40 */
   /* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */
   static const Id checksum = Id(
      BEIDN_BGLGEN_SQL_CT_REGISTRY_INFO
      BEIDN_BGLGEN_SQL_CT_COMMENTS
      BEIDN_BGLGEN_SQL_CT_APIS
      BEIDN_BGLGEN_SQL_CT_PROFILES
      BEIDN_BGLGEN_SQL_CT_TYPES
      BEIDN_BGLGEN_SQL_CT_TYPE_DECLARATIONS
      BEIDN_BGLGEN_SQL_CT_TYPE_DEPENDENCIES
      BEIDN_BGLGEN_SQL_CT_GROUPS
      BEIDN_BGLGEN_SQL_CT_GROUP_ENUMS
      BEIDN_BGLGEN_SQL_CT_ENUMS
      BEIDN_BGLGEN_SQL_CT_ENUM_DECLARATIONS
      BEIDN_BGLGEN_SQL_CT_ENUM_ALIASES
      BEIDN_BGLGEN_SQL_CT_COMMANDS
      BEIDN_BGLGEN_SQL_CT_COMMAND_SIGNATURE
      BEIDN_BGLGEN_SQL_CT_COMMAND_ALIASES
      BEIDN_BGLGEN_SQL_CT_COMPONENTS
      BEIDN_BGLGEN_SQL_CT_COMPONENT_APIS
      BEIDN_BGLGEN_SQL_CT_COMPONENT_TYPES
      BEIDN_BGLGEN_SQL_CT_COMPONENT_ENUMS
      BEIDN_BGLGEN_SQL_CT_COMPONENT_COMMANDS
      BEIDN_BGLGEN_SQL_CI_ENUM_ALIASES_REVERSE
      BEIDN_BGLGEN_SQL_CI_ENUM_GROUP
      BEIDN_BGLGEN_SQL_CI_COMMAND_ALIASES_REVERSE
      BEIDN_BGLGEN_SQL_CI_API_COMPONENT
      BEIDN_BGLGEN_SQL_CI_TYPE_COMPONENT
      BEIDN_BGLGEN_SQL_CI_ENUM_COMPONENT
      BEIDN_BGLGEN_SQL_CI_COMMAND_COMPONENT
      BEIDN_BGLGEN_SQL_CV_FEATURES
      BEIDN_BGLGEN_SQL_CV_EXTENSIONS
      BEIDN_BGLGEN_SQL_CV_TYPE_COMPONENTS
      BEIDN_BGLGEN_SQL_CV_ENUM_COMPONENTS
      BEIDN_BGLGEN_SQL_CV_COMMAND_COMPONENTS
      BEIDN_BGLGEN_SQL_CV_API_TYPES
      BEIDN_BGLGEN_SQL_CV_API_ENUMS
      BEIDN_BGLGEN_SQL_CV_API_GROUP_ENUMS
   );
   /* ######################### END OF GENERATED CODE ######################### */

   return checksum;
}

//////////////////////////////////////////////////////////////////////////////
void clear_registry_db(sqlite::Db& db) {
   be_short_info("") << "Rebuilding registry DB..." | default_log();

   sqlite::Transaction tx(db);

   sqlite::exec(db, "UPDATE registry_info SET xml_checksum = 0, xml_size = 0, xml_path = ''");

   /*!!
   register_template_string([[`with each $ using # { nl `sqlite::exec(db, "DROP INDEX IF EXISTS `$`_index");`} nl]], 'drop_indices')
   register_template_string([[`with each $ using # { nl `sqlite::exec(db, "DELETE FROM `$`");`} nl]], 'truncate')
   write_template('drop_indices', indices)
   write_template('truncate', tables)
   write_template('exec_ids', index_ids) 
   !! 40 */
   /* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */

   sqlite::exec(db, "DROP INDEX IF EXISTS enum_aliases_reverse_index");
   sqlite::exec(db, "DROP INDEX IF EXISTS enum_group_index");
   sqlite::exec(db, "DROP INDEX IF EXISTS command_aliases_reverse_index");
   sqlite::exec(db, "DROP INDEX IF EXISTS api_component_index");
   sqlite::exec(db, "DROP INDEX IF EXISTS type_component_index");
   sqlite::exec(db, "DROP INDEX IF EXISTS enum_component_index");
   sqlite::exec(db, "DROP INDEX IF EXISTS command_component_index");

   sqlite::exec(db, "DELETE FROM comments");
   sqlite::exec(db, "DELETE FROM apis");
   sqlite::exec(db, "DELETE FROM profiles");
   sqlite::exec(db, "DELETE FROM types");
   sqlite::exec(db, "DELETE FROM type_declarations");
   sqlite::exec(db, "DELETE FROM type_dependencies");
   sqlite::exec(db, "DELETE FROM groups");
   sqlite::exec(db, "DELETE FROM group_enums");
   sqlite::exec(db, "DELETE FROM enums");
   sqlite::exec(db, "DELETE FROM enum_declarations");
   sqlite::exec(db, "DELETE FROM enum_aliases");
   sqlite::exec(db, "DELETE FROM commands");
   sqlite::exec(db, "DELETE FROM command_signature");
   sqlite::exec(db, "DELETE FROM command_aliases");
   sqlite::exec(db, "DELETE FROM components");
   sqlite::exec(db, "DELETE FROM component_apis");
   sqlite::exec(db, "DELETE FROM component_types");
   sqlite::exec(db, "DELETE FROM component_enums");
   sqlite::exec(db, "DELETE FROM component_commands");

   sqlite::exec(db, BEIDN_BGLGEN_SQL_CI_ENUM_ALIASES_REVERSE);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CI_ENUM_GROUP);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CI_COMMAND_ALIASES_REVERSE);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CI_API_COMPONENT);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CI_TYPE_COMPONENT);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CI_ENUM_COMPONENT);
   sqlite::exec(db, BEIDN_BGLGEN_SQL_CI_COMMAND_COMPONENT);

   /* ######################### END OF GENERATED CODE ######################### */

   sqlite::exec(db, "DELETE FROM sqlite_sequence");

   tx.commit();
}

//////////////////////////////////////////////////////////////////////////////
void parse_comment(sqlite::StmtCache& cache, pugi::xml_node node) {
   auto s = cache.obtain("INSERT INTO comments (comment) VALUES (?)");
   s.bind(1, node.child_value());
   s.step();
}

//////////////////////////////////////////////////////////////////////////////
U64 next_api_id() {
   static U64 id = 0;
   return ++id;
}

//////////////////////////////////////////////////////////////////////////////
U64 get_api_id(sqlite::StmtCache& cache, const char* api) {
   auto s = cache.obtain("SELECT id FROM apis WHERE name = ?");
   s.bind(1, api);
   if (s.step()) {
      return s.get_u64(0);
   } else {
      be_short_verbose("") << "API: " << S(api) | default_log();

      U64 id = next_api_id();
      auto i = cache.obtain("INSERT INTO apis (id, name) VALUES (?, ?)");
      i.bind(1, id);
      i.bind(2, api);
      i.step();
      return id;
   }
}

//////////////////////////////////////////////////////////////////////////////
U64 next_profile_id() {
   static U64 id = 0;
   return ++id;
}

//////////////////////////////////////////////////////////////////////////////
U64 get_profile_id(sqlite::StmtCache& cache, const char* profile) {
   auto s = cache.obtain("SELECT id FROM profiles WHERE name = ?");
   s.bind(1, profile);
   if (s.step()) {
      return s.get_u64(0);
   } else {
      be_short_verbose("") << "Profile: " << S(profile) | default_log();

      U64 id = next_profile_id();
      auto i = cache.obtain("INSERT INTO profiles (id, name) VALUES (?, ?)");
      i.bind(1, id);
      i.bind(2, profile);
      i.step();
      return id;
   }
}

//////////////////////////////////////////////////////////////////////////////
U64 next_group_id() {
   static U64 id = 0;
   return ++id;
}

//////////////////////////////////////////////////////////////////////////////
U64 get_group_id(sqlite::StmtCache& cache, const char* group) {
   auto s = cache.obtain("SELECT id FROM groups WHERE name = ?");
   s.bind(1, group);
   if (s.step()) {
      return s.get_u64(0);
   } else {
      be_short_verbose("") << "Group: " << S(group) | default_log();

      U64 id = next_group_id();

      auto i = cache.obtain("INSERT INTO groups (id, name) VALUES (?, ?)");
      i.bind(1, id);
      i.bind(2, group);
      i.step();
      return id;
   }
}

//////////////////////////////////////////////////////////////////////////////
U64 next_type_id() {
   static U64 id = 0;
   return ++id;
}

//////////////////////////////////////////////////////////////////////////////
void parse_types(sqlite::StmtCache& cache, pugi::xml_node node) {
   auto select = cache.obtain("SELECT id FROM types WHERE name = ?");
   auto insert = cache.obtain("INSERT INTO types (id, name) VALUES (?, ?)");
   auto insert_decl = cache.obtain("INSERT INTO type_declarations (type_id, api_id, declaration, comment) VALUES (?, ?, ?, ?)");
   auto insert_dep = cache.obtain("INSERT OR IGNORE INTO type_dependencies (dependent_type_id, nondependent_type_id, api_id) VALUES (?, ?, ?)");
   for (pugi::xml_node type : node.children("type")) {
      const char* name;
      U64 id;
      if (auto name_element = type.child("name"); !name_element.empty()) {
         name = name_element.child_value();
      } else if (auto name_attrib = type.attribute("name"); !name_attrib.empty()) {
         name = name_attrib.value();
      } else {
         continue;
      }

      select.reset();
      select.bind(1, name);
      if (select.step()) {
         id = select.get_u64(0);
      } else {
         be_short_verbose("") << "Type: " << S(name) | default_log();

         id = next_type_id();
         insert.reset();
         insert.bind();
         insert.bind(1, id);
         insert.bind(2, name);
         insert.step();
      }

      const char* api = nullptr;
      if (pugi::xml_attribute attr = type.attribute("api"); !attr.empty()) {
         api = attr.value();
      }

      const char* comment = nullptr;
      if (pugi::xml_attribute attr = type.attribute("comment"); !attr.empty()) {
         comment = attr.value();
      }
      
      S declaration;
      for (pugi::xml_node child : type.children()) {
         if (child.type() == pugi::node_element) {
            if (0 == strcmp(child.name(), "apientry")) {
               declaration += "\xEF\xBF\xBD";
            } else {
               declaration += child.child_value();
            }
         } else {
            declaration += child.value();
         }
      }

      insert_decl.reset();
      insert_decl.bind();
      insert_decl.bind(1, id);
      if (api) insert_decl.bind(2, get_api_id(cache, api));
      insert_decl.bind(3, declaration.c_str());
      if (comment) insert_decl.bind(4, comment);
      insert_decl.step();

      if (pugi::xml_attribute requires = type.attribute("requires"); !requires.empty()) {
         select.reset();
         select.bind(1, requires.value());
         if (select.step()) {
            U64 dependency = select.get_u64(0);

            insert_dep.reset();
            insert_dep.bind();
            insert_dep.bind(1, id);
            insert_dep.bind(2, dependency);
            if (api) insert_dep.bind(3, get_api_id(cache, api));
            insert_dep.step();

         } else {
            be_warn() << "Type 'requires' attribute references undefined type."
               & attr("Dependent Type") << S(name)
               & attr("Nondependent Type") << S(requires.value())
               | default_log();
         }
      }
   }
}

//////////////////////////////////////////////////////////////////////////////
void parse_groups(pugi::xml_node node, std::map<S, std::set<S>>& groups) {
   for (pugi::xml_node group : node.children("group")) {
      const char* group_name = group.attribute("name").value();

      for (pugi::xml_node enum_elem : group.children("enum")) {
         const char* enum_name = enum_elem.attribute("name").value();

         groups[S(enum_name)].insert(S(group_name));
      }
   }
}

//////////////////////////////////////////////////////////////////////////////
U64 next_enum_id() {
   static U64 id = 0;
   return ++id;
}

//////////////////////////////////////////////////////////////////////////////
void parse_enums(sqlite::StmtCache& cache, pugi::xml_node node, const std::map<S, std::set<S>>& groups) {
   auto get_type = cache.obtain("SELECT id FROM types WHERE name = ?");
   auto get_enum = cache.obtain("SELECT id FROM enums WHERE name = ?");
   auto insert_group_enum = cache.obtain("INSERT INTO group_enums (group_id, enum_id) VALUES (?, ?)");
   auto insert_enum = cache.obtain("INSERT INTO enums (id, name, bitmask) VALUES (?, ?, ?)");
   auto insert_decl = cache.obtain("INSERT INTO enum_declarations (enum_id, api_id, value, raw_value, suffix, comment) VALUES (?, ?, ?, ?, ?, ?)");
   auto insert_alias = cache.obtain("INSERT INTO enum_aliases (preferred_id, alias_id) VALUES (?, ?)");

   const char* enum_type_name = node.attribute("type").value();
   bool bitmask = (0 == strcmp(enum_type_name, "bitmask"));

   for (pugi::xml_node enum_elem : node.children("enum")) {
      const char* enum_name = enum_elem.attribute("name").value();

      const char* api = nullptr;
      if (pugi::xml_attribute attr = enum_elem.attribute("api"); !attr.empty()) {
         api = attr.value();
      }

      const char* suffix = nullptr;
      if (pugi::xml_attribute attr = enum_elem.attribute("type"); !attr.empty()) {
         suffix = attr.value();
      }

      const char* comment = nullptr;
      if (pugi::xml_attribute attr = enum_elem.attribute("comment"); !attr.empty()) {
         comment = attr.value();
      }

      const char* raw_value = enum_elem.attribute("value").value();
      char* value_end = nullptr;
      I64 value = std::strtoll(raw_value, &value_end, 0);

      U64 id;

      get_enum.reset();
      get_enum.bind(1, enum_name);
      if (get_enum.step()) {
         id = get_enum.get_u64(0);
      } else {
         be_short_verbose("") << "Enum: " << S(enum_name) | default_log();

         id = next_enum_id();
         insert_enum.reset();
         insert_enum.bind(1, id);
         insert_enum.bind(2, enum_name);
         insert_enum.bind(3, bitmask);
         insert_enum.step();

         if (auto group_it = groups.find(S(enum_name)); group_it != groups.end()) {
            const std::set<S>& set = group_it->second;
            for (const S& group_name : set) {
               U64 group_id = get_group_id(cache, group_name.c_str());
               insert_group_enum.reset();
               insert_group_enum.bind(1, group_id);
               insert_group_enum.bind(2, id);
               insert_group_enum.step();
            }
         }
      }

      insert_decl.reset();
      insert_decl.bind();
      insert_decl.bind(1, id);
      if (api) insert_decl.bind(2, get_api_id(cache, api));
      if (value_end && 0 == *value_end) insert_decl.bind(3, value);
      insert_decl.bind(4, raw_value);
      insert_decl.bind(5, suffix);
      insert_decl.bind(6, comment);
      insert_decl.step();

      if (pugi::xml_attribute alias = enum_elem.attribute("alias"); !alias.empty()) {
         S alias_name = alias.value();
         if (alias_name.length() < 3 || alias_name[0] != 'G' || alias_name[1] != 'L' || alias_name[2] != '_') {
            alias_name = "GL_" + alias_name;
         }

         U64 alias_id = U64(-1);
         get_enum.reset();
         get_enum.bind(1, alias_name.c_str());
         if (get_enum.step()) {
            alias_id = get_enum.get_u64(0);
         } else {
            be_warn() << "Enum 'alias' attribute references undefined enum."
               & attr("Aliasing Enum") << S(enum_name)
               & attr("Aliased Enum") << S(alias_name)
               | default_log();
            alias_name = S();
         }

         if (!alias_name.empty()) {
            insert_alias.reset();
            insert_alias.bind(1, alias_id);
            insert_alias.bind(2, id);
            insert_alias.step();
         }
      }
   }
}

//////////////////////////////////////////////////////////////////////////////
U64 next_command_id() {
   static U64 id = 0;
   return ++id;
}

//////////////////////////////////////////////////////////////////////////////
void parse_commands(sqlite::StmtCache& cache, pugi::xml_node node, std::vector<std::pair<U64, S>>& lazy_aliases, std::vector<std::pair<U64, S>>& lazy_vecequivs) {
   auto get_type = cache.obtain("SELECT id FROM types WHERE name = ?");
   auto get_command = cache.obtain("SELECT id FROM commands WHERE name = ?");
   auto insert_command = cache.obtain("INSERT INTO commands (id, name, comment) VALUES (?, ?, ?)");
   auto insert_alias = cache.obtain("INSERT INTO command_aliases (preferred_id, alias_id, type) VALUES (?, ?, ?)");
   auto insert_signature = cache.obtain("INSERT INTO command_signature (command_id, param_index, type_id, group_id, length, raw_length, declaration, name) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

   for (pugi::xml_node command_elem : node.children("command")) {
      pugi::xml_node proto = command_elem.child("proto");
      const char* command_name = proto.child("name").child_value();

      const char* comment = nullptr;
      if (pugi::xml_attribute attr = command_elem.attribute("comment"); !attr.empty()) {
         comment = attr.value();
      }

      U64 id = next_command_id();

      be_short_verbose("") << "Group: " << S(command_name) | default_log();

      insert_command.reset();
      insert_command.bind();
      insert_command.bind(1, id);
      insert_command.bind(2, command_name);
      insert_command.bind(3, comment);
      insert_command.step();
      
      U64 type_id = U64(-1);
      if (auto ptype = proto.child("ptype"); !ptype.empty()) {
         get_type.reset();
         get_type.bind(1, ptype.child_value());
         if (get_type.step()) {
            type_id = get_type.get_u64(0);
         } else {
            be_warn() << "Command 'ptype' element references undefined type."
               & attr("Command") << S(command_name)
               & attr("Type") << S(ptype.child_value())
               | default_log();
         }
      }

      U64 group_id = U64(-1);
      if (auto group = proto.attribute("group"); !group.empty()) {
         group_id = get_group_id(cache, group.value());
      }

      S return_decl;
      for (auto proto_node : proto.children()) {
         if (proto_node.type() != pugi::node_element) {
            return_decl.append(proto_node.value());
         } else if (0 == strcmp(proto_node.name(), "name")) {
            break;
         } else {
            return_decl.append(proto_node.child_value());
         }
      }

      insert_signature.reset();
      insert_signature.bind();
      insert_signature.bind(1, id);
      insert_signature.bind(2, 0);
      if (type_id != U64(-1)) insert_signature.bind(3, type_id);
      if (group_id != U64(-1)) insert_signature.bind(4, group_id);
      insert_signature.bind(7, return_decl.c_str(), ( int ) return_decl.size());
      insert_signature.bind(8, command_name);
      insert_signature.step();

      I32 param_index = 1;
      for (auto param_node : command_elem.children()) {
         if (param_node.type() == pugi::node_element) {
            if (0 == strcmp(param_node.name(), "param")) {
               
               type_id = U64(-1);
               if (auto ptype = param_node.child("ptype"); !ptype.empty()) {
                  get_type.reset();
                  get_type.bind(1, ptype.child_value());
                  if (get_type.step()) {
                     type_id = get_type.get_u64(0);
                  } else {
                     be_warn() << "Command parameter 'ptype' element references undefined type."
                        & attr("Command") << S(command_name)
                        & attr("Parameter Index") << param_index
                        & attr("Type") << S(ptype.child_value())
                        | default_log();
                  }
               }

               group_id = U64(-1);
               if (auto group = param_node.attribute("group"); !group.empty()) {
                  group_id = get_group_id(cache, group.value());
               }

               const char* raw_length = nullptr;
               char* length_end = nullptr;
               I64 length = 0;
               if (auto len = param_node.attribute("len"); !len.empty()) {
                  raw_length = len.value();
                  length = std::strtoll(raw_length, &length_end, 0);
               }

               S decl;
               const char* param_name = nullptr;
               for (auto param_child : param_node.children()) {
                  if (param_child.type() != pugi::node_element) {
                     decl.append(param_child.value());
                  } else if (0 == strcmp(param_child.name(), "name")) {
                     param_name = param_child.child_value();
                     break;
                  } else {
                     decl.append(param_child.child_value());
                  }
               }

               insert_signature.reset();
               insert_signature.bind();
               insert_signature.bind(1, id);
               insert_signature.bind(2, param_index);
               if (type_id != U64(-1)) insert_signature.bind(3, type_id);
               if (group_id != U64(-1)) insert_signature.bind(4, group_id);
               if (length_end && 0 == *length_end) insert_signature.bind(5, length);
               insert_signature.bind(6, raw_length);
               insert_signature.bind(7, decl.c_str(), ( int ) decl.size());
               insert_signature.bind(8, param_name);
               insert_signature.step();

               ++param_index;

            } else if (0 == strcmp(param_node.name(), "alias")) {
               const char* alias_name = param_node.attribute("name").value();
               get_command.reset();
               get_command.bind(1, alias_name);
               if (get_command.step()) {
                  U64 alias_id = get_command.get_u64(0);
                  insert_alias.reset();
                  insert_alias.bind(1, alias_id);
                  insert_alias.bind(2, id);
                  insert_alias.bind(3, 0);
                  insert_alias.step();
               } else {
                  lazy_aliases.push_back(std::make_pair(id, S(alias_name)));
               }

            } else if (0 == strcmp(param_node.name(), "vecequiv")) {
               const char* alias_name = param_node.attribute("name").value();
               get_command.reset();
               get_command.bind(1, alias_name);
               if (get_command.step()) {
                  U64 alias_id = get_command.get_u64(0);
                  insert_alias.reset();
                  insert_alias.bind(1, alias_id);
                  insert_alias.bind(2, id);
                  insert_alias.bind(3, 1);
                  insert_alias.step();
               } else {
                  lazy_vecequivs.push_back(std::make_pair(id, S(alias_name)));
               }
            }
         }
      }
   }
}

//////////////////////////////////////////////////////////////////////////////
U64 next_component_id() {
   static U64 id = 0;
   return ++id;
}

//////////////////////////////////////////////////////////////////////////////
void parse_feature(sqlite::StmtCache& cache, pugi::xml_node node) {
   auto get_type = cache.obtain("SELECT id FROM types WHERE name = ?");
   auto get_enum = cache.obtain("SELECT id FROM enums WHERE name = ?");
   auto get_command = cache.obtain("SELECT id FROM commands WHERE name = ?");
   auto insert = cache.obtain("INSERT INTO components (id, name, type, version_major, version_minor, comment) VALUES (?, ?, ?, ?, ?, ?)");
   auto insert_api = cache.obtain("INSERT INTO component_apis (component_id, api_id) VALUES (?, ?)");
   auto insert_type = cache.obtain("INSERT INTO component_types (component_id, type_id, api_id, profile_id, op, comment) VALUES (?, ?, ?, ?, ?, ?)");
   auto insert_enum = cache.obtain("INSERT INTO component_enums (component_id, enum_id, api_id, profile_id, op, comment) VALUES (?, ?, ?, ?, ?, ?)");
   auto insert_command = cache.obtain("INSERT INTO component_commands (component_id, command_id, api_id, profile_id, op, comment) VALUES (?, ?, ?, ?, ?, ?)");

   const char* feature_name = node.attribute("name").value();
   const char* api = node.attribute("api").value();

   const char* number = nullptr;
   U8 major = 0;
   U8 minor = 0;
   if (pugi::xml_attribute attr = node.attribute("number"); !attr.empty()) {
      number = attr.value();
      U8* ptr = &major;
      for (const char* it = number; *it; ++it) {
         char c = *it;
         if (c >= '0' && c <= '9') {
            *ptr = *ptr * 10 + (c - '0');
         } else if (c == '.' && ptr == &major) {
            ptr = &minor;
         } else if (c != ' ' && c != '\t') {
            major = 0;
            minor = 0;
            break;
         }
      }
   }

   const char* comment = nullptr;
   if (pugi::xml_attribute attr = node.attribute("comment"); !attr.empty()) {
      comment = attr.value();
   }

   U64 id = next_component_id();

   insert.bind(1, id);
   insert.bind(2, feature_name);
   insert.bind(3, 0);
   insert.bind(4, major);
   insert.bind(5, minor);
   insert.bind(6, comment);
   insert.step();

   insert_api.bind(1, id);
   insert_api.bind(2, get_api_id(cache, api));
   insert_api.step();

   for (pugi::xml_node child : node.children()) {
      if (child.type() != pugi::node_element) {
         continue;
      }

      bool op;
      if (const char* tag = child.name(); 0 == strcmp(tag, "require")) {
         op = true;
      } else if (0 == strcmp(tag, "remove")) {
         op = false;
      } else {
         continue;
      }

      const char* profile = nullptr;
      if (pugi::xml_attribute attr = child.attribute("profile"); !attr.empty()) {
         profile = attr.value();
      }

      for (pugi::xml_node grandchild : child.children()) {
         const char* name = grandchild.attribute("name").value();
         const char* subcomment = nullptr;
         if (pugi::xml_attribute attr = grandchild.attribute("comment"); !attr.empty()) {
            subcomment = attr.value();
         }

         if (const char* tag = grandchild.name(); 0 == strcmp(tag, "command")) {
            U64 command_id;

            get_command.reset();
            get_command.bind(1, name);
            if (get_command.step()) {
               command_id = get_command.get_u64(0);
            } else {
               be_warn() << "Feature 'command' element references undefined command."
                  & attr("Feature") << S(feature_name)
                  & attr("Command") << S(name)
                  | default_log();
               continue;
            }

            insert_command.reset();
            insert_command.bind();
            insert_command.bind(1, id);
            insert_command.bind(2, command_id);
            insert_command.bind(3, get_api_id(cache, api));
            if (profile) insert_command.bind(4, get_profile_id(cache, profile));
            insert_command.bind(5, op);
            insert_command.bind(6, subcomment);
            insert_command.step();

         } else if (0 == strcmp(tag, "enum")) {
            U64 enum_id;

            get_enum.reset();
            get_enum.bind(1, name);
            if (get_enum.step()) {
               enum_id = get_enum.get_u64(0);
            } else {
               be_warn() << "Feature 'enum' element references undefined enum."
                  & attr("Feature") << S(feature_name)
                  & attr("Enum") << S(name)
                  | default_log();
               continue;
            }

            insert_enum.reset();
            insert_enum.bind();
            insert_enum.bind(1, id);
            insert_enum.bind(2, enum_id);
            insert_enum.bind(3, get_api_id(cache, api));
            if (profile) insert_enum.bind(4, get_profile_id(cache, profile));
            insert_enum.bind(5, op);
            insert_enum.bind(6, subcomment);
            insert_enum.step();

         } else if (0 == strcmp(tag, "type")) {
            U64 type_id;

            get_type.reset();
            get_type.bind(1, name);
            if (get_type.step()) {
               type_id = get_type.get_u64(0);
            } else {
               be_warn() << "Feature 'type' element references undefined type."
                  & attr("Feature") << S(feature_name)
                  & attr("Type") << S(name)
                  | default_log();
               continue;
            }

            insert_type.reset();
            insert_type.bind();
            insert_type.bind(1, id);
            insert_type.bind(2, type_id);
            insert_type.bind(3, get_api_id(cache, api));
            if (profile) insert_type.bind(4, get_profile_id(cache, profile));
            insert_type.bind(5, op);
            insert_type.bind(6, subcomment);
            insert_type.step();
         }
      }
   }
}

//////////////////////////////////////////////////////////////////////////////
void parse_extensions(sqlite::StmtCache& cache, pugi::xml_node extensions) {
   auto get_type = cache.obtain("SELECT id FROM types WHERE name = ?");
   auto get_enum = cache.obtain("SELECT id FROM enums WHERE name = ?");
   auto get_command = cache.obtain("SELECT id FROM commands WHERE name = ?");
   auto insert = cache.obtain("INSERT INTO components (id, name, type, version_major, version_minor, comment) VALUES (?, ?, ?, ?, ?, ?)");
   auto insert_api = cache.obtain("INSERT INTO component_apis (component_id, api_id) VALUES (?, ?)");
   auto insert_type = cache.obtain("INSERT INTO component_types (component_id, type_id, api_id, profile_id, op, comment) VALUES (?, ?, ?, ?, ?, ?)");
   auto insert_enum = cache.obtain("INSERT INTO component_enums (component_id, enum_id, api_id, profile_id, op, comment) VALUES (?, ?, ?, ?, ?, ?)");
   auto insert_command = cache.obtain("INSERT INTO component_commands (component_id, command_id, api_id, profile_id, op, comment) VALUES (?, ?, ?, ?, ?, ?)");

   for (pugi::xml_node extension : extensions.children("extension")) {
      const char* extension_name = extension.attribute("name").value();

      const char* comment = nullptr;
      if (pugi::xml_attribute attr = extension.attribute("comment"); !attr.empty()) {
         comment = attr.value();
      }

      U64 id = next_component_id();

      insert.reset();
      insert.bind();
      insert.bind(1, id);
      insert.bind(2, extension_name);
      insert.bind(3, 1);
      insert.bind(6, comment);
      insert.step();

      {
         S supported = extension.attribute("supported").value();
         std::size_t start = 0, end = 0;
         while ((end = supported.find('|', start)) != S::npos) {
            S api = supported.substr(start, end - start);
            start = end + 1;

            insert_api.reset();
            insert_api.bind(1, id);
            insert_api.bind(2, get_api_id(cache, api.c_str()));
            insert_api.step();
         }

         S api = supported.substr(start);

         insert_api.reset();
         insert_api.bind(1, id);
         insert_api.bind(2, get_api_id(cache, api.c_str()));
         insert_api.step();
      }
      

      for (pugi::xml_node child : extension.children()) {
         if (child.type() != pugi::node_element) {
            continue;
         }

         bool op;
         if (const char* tag = child.name(); 0 == strcmp(tag, "require")) {
            op = true;
         } else if (0 == strcmp(tag, "remove")) {
            op = false;
         } else {
            continue;
         }

         const char* profile = nullptr;
         if (pugi::xml_attribute attr = child.attribute("profile"); !attr.empty()) {
            profile = attr.value();
         }

         const char* api = nullptr;
         if (pugi::xml_attribute attr = child.attribute("api"); !attr.empty()) {
            api = attr.value();
         }

         for (pugi::xml_node grandchild : child.children()) {
            const char* name = grandchild.attribute("name").value();
            const char* subcomment = nullptr;
            if (pugi::xml_attribute attr = grandchild.attribute("comment"); !attr.empty()) {
               subcomment = attr.value();
            }

            if (const char* tag = grandchild.name(); 0 == strcmp(tag, "command")) {
               U64 command_id;

               get_command.reset();
               get_command.bind(1, name);
               if (get_command.step()) {
                  command_id = get_command.get_u64(0);
               } else {
                  be_warn() << "Extension 'command' element references undefined command."
                     & attr("Extension") << S(extension_name)
                     & attr("Command") << S(name)
                     | default_log();
                  continue;
               }

               insert_command.reset();
               insert_command.bind();
               insert_command.bind(1, id);
               insert_command.bind(2, command_id);
               if (api) insert_command.bind(3, get_api_id(cache, api));
               if (profile) insert_command.bind(4, get_profile_id(cache, profile));
               insert_command.bind(5, op);
               if (subcomment) insert_command.bind(6, subcomment);
               insert_command.step();

            } else if (0 == strcmp(tag, "enum")) {
               U64 enum_id;

               get_enum.reset();
               get_enum.bind(1, name);
               if (get_enum.step()) {
                  enum_id = get_enum.get_u64(0);
               } else {
                  be_warn() << "Extension 'enum' element references undefined enum."
                     & attr("Extension") << S(extension_name)
                     & attr("Enum") << S(name)
                     | default_log();
                  continue;
               }

               insert_enum.reset();
               insert_enum.bind();
               insert_enum.bind(1, id);
               insert_enum.bind(2, enum_id);
               if (api) insert_enum.bind(3, get_api_id(cache, api));
               if (profile) insert_enum.bind(4, get_profile_id(cache, profile));
               insert_enum.bind(5, op);
               if (subcomment) insert_enum.bind(6, subcomment);
               insert_enum.step();

            } else if (0 == strcmp(tag, "type")) {
               U64 type_id;

               get_type.reset();
               get_type.bind(1, name);
               if (get_type.step()) {
                  type_id = get_type.get_u64(0);
               } else {
                  be_warn() << "Extension 'type' element references undefined type."
                     & attr("Extension") << S(extension_name)
                     & attr("Type") << S(name)
                     | default_log();
                  continue;
               }

               insert_type.reset();
               insert_type.bind();
               insert_type.bind(1, id);
               insert_type.bind(2, type_id);
               if (api) insert_type.bind(3, get_api_id(cache, api));
               if (profile) insert_type.bind(4, get_profile_id(cache, profile));
               insert_type.bind(5, op);
               if (subcomment) insert_type.bind(6, subcomment);
               insert_type.step();
            }
         }
      }
   }
}

} // be::bglgen::()

//////////////////////////////////////////////////////////////////////////////
sqlite::Db init_registry(const Path& xml_path, const Path& db_path, bool force_rebuild) {
   sqlite::Db db;

   be_short_verbose("") << "Registry Location: " << xml_path.generic_string() | default_log();
   be_short_verbose("") << "Registry DB Location: " << db_path.generic_string() | default_log();

   S raw_xml = util::get_file_contents_string(xml_path);
   Id xml_checksum = Id(raw_xml);

   bool delete_db = false;
   bool create_db = false;
   bool clear_db = false;
   bool parse_xml = false;

   if (":memory:" == db_path) {
      db = sqlite::Db(":memory:");
      create_db = true;
      parse_xml = true;
   } else if (fs::exists(db_path)) {
      db = sqlite::Db(db_path.string());
      sqlite::Stmt s_info_exists = sqlite::Stmt(db, "SELECT 1 FROM sqlite_master WHERE type = 'table' AND name = 'registry_info' LIMIT 1");
      if (!s_info_exists.step()) {
         be_short_info("") << "Registry DB appears to be corrupt!" | default_log();
         delete_db = true;
         create_db = true;
         parse_xml = true;
      } else {
         sqlite::Stmt s_checksum = sqlite::Stmt(db, "SELECT schema_checksum, xml_checksum FROM registry_info LIMIT 1");
         if (s_checksum.step()) {
            Id schema_checksum = Id(s_checksum.get_u64(0));
            if (schema_checksum != get_schema_checksum()) {
               be_short_info("") << "Registry DB schema is out of date." | default_log();
               delete_db = true;
               create_db = true;
               parse_xml = true;
            } else {
               Id sqlite_xml_checksum = Id(s_checksum.get_u64(1));
               if (sqlite_xml_checksum != xml_checksum) {
                  be_short_info("") << "Registry DB is out of date." | default_log();
                  clear_db = true;
                  parse_xml = true;
               }
            }
         } else {
            be_short_info("") << "Registry DB appears to be corrupt!" | default_log();
            delete_db = true;
            create_db = true;
            parse_xml = true;
         }
      }
   } else {
      db = sqlite::Db(db_path.string());
      create_db = true;
      parse_xml = true;
   }

   if (force_rebuild && !create_db) {
      clear_db = true;
      parse_xml = true;
   }

   if (delete_db) {
      db = sqlite::Db();
      be_short_verbose("") << "Deleting registry DB..." | default_log();
      fs::remove(db_path);
      db = sqlite::Db(db_path.string());
   }

   sqlite::StaticStmtCache cache(db);

   if (create_db) {
      create_registry_db(db);
   }

   if (clear_db) {
      clear_registry_db(db);
   }

   if (parse_xml) {
      pugi::xml_document doc;
      doc.load_buffer_inplace(raw_xml.data(), raw_xml.size());

      pugi::xml_node root = doc.root().child("registry");

      if (root.empty()) {
         throw std::runtime_error("Could not find root <registry> element in gl.xml!");
      }

      enum element_type {
         t_comment = 1, t_types, t_groups, t_enums, t_commands, t_feature, t_extensions
      };

      auto parser = util::ExactKeywordParser<element_type>();
      parser
         (t_comment, "comment")
         (t_types, "types")
         (t_groups, "groups")
         (t_enums, "enums")
         (t_commands, "commands")
         (t_feature, "feature")
         (t_extensions, "extensions");

      std::map<S, std::set<S>> enum_groups; // enum name -> group name
      std::vector<std::pair<U64, S>> lazy_command_aliases;
      std::vector<std::pair<U64, S>> lazy_command_vecequivs;

      element_type last_type = static_cast<element_type>(0);
      for (pugi::xml_node node : root.children()) {
         if (node.type() != pugi::node_element) {
            continue;
         }

         sqlite::Transaction tx(db);

         auto type = parser.parse(node.name());

         if (type != last_type) {
            switch (type) {
               case t_comment:    be_short_info("") << "Parsing comments..." | default_log(); break;
               case t_types:      be_short_info("") << "Parsing types..." | default_log(); break;
               case t_groups:     be_short_info("") << "Parsing groups..." | default_log(); break;
               case t_enums:      be_short_info("") << "Parsing enums..." | default_log(); break;
               case t_commands:   be_short_info("") << "Parsing commands..." | default_log(); enum_groups.clear(); break;
               case t_feature:    be_short_info("") << "Parsing features..." | default_log(); break;
               case t_extensions: be_short_info("") << "Parsing extensions..." | default_log(); break;
            }

            if (type == t_feature) {
               auto get_command = cache.obtain("SELECT id FROM commands WHERE name = ?");
               auto get_command_name = cache.obtain("SELECT name FROM commands WHERE id = ?");
               auto insert_alias = cache.obtain("INSERT INTO command_aliases (preferred_id, alias_id, type) VALUES (?, ?, ?)");

               for (auto& p : lazy_command_aliases) {
                  const char* alias_name = p.second.c_str();
                  get_command.reset();
                  get_command.bind(1, alias_name);
                  if (get_command.step()) {
                     U64 alias_id = get_command.get_u64(0);
                     insert_alias.reset();
                     insert_alias.bind(1, alias_id);
                     insert_alias.bind(2, p.first);
                     insert_alias.bind(3, 0);
                     insert_alias.step();
                  } else {
                     get_command_name.reset();
                     get_command_name.bind(1, p.first);
                     get_command_name.step();
                     const char* command_name = get_command_name.get_text(0);
                     be_warn() << "Command 'alias' element references undefined command."
                        & attr("Aliasing Command") << S(command_name)
                        & attr("Aliased Command") << S(alias_name)
                        | default_log();
                  }
               }
               lazy_command_aliases.clear();

               for (auto& p : lazy_command_vecequivs) {
                  const char* alias_name = p.second.c_str();
                  get_command.reset();
                  get_command.bind(1, alias_name);
                  if (get_command.step()) {
                     U64 alias_id = get_command.get_u64(0);
                     insert_alias.reset();
                     insert_alias.bind(1, alias_id);
                     insert_alias.bind(2, p.first);
                     insert_alias.bind(3, 1);
                     insert_alias.step();
                  } else {
                     get_command_name.reset();
                     get_command_name.bind(1, p.first);
                     get_command_name.step();
                     const char* command_name = get_command_name.get_text(0);
                     be_warn() << "Command 'vecequiv' element references undefined command."
                        & attr("Scalar Command") << S(command_name)
                        & attr("Vector Command") << S(alias_name)
                        | default_log();
                  }
               }
               lazy_command_vecequivs.clear();
            }

            last_type = type;
         }

         switch (type) {
            case t_comment:    parse_comment(cache, node); break;
            case t_types:      parse_types(cache, node); break;
            case t_groups:     parse_groups(node, enum_groups); break;
            case t_enums:      parse_enums(cache, node, enum_groups); break;
            case t_commands:   parse_commands(cache, node, lazy_command_aliases, lazy_command_vecequivs); break;
            case t_feature:    parse_feature(cache, node); break;
            case t_extensions: parse_extensions(cache, node); break;
         }

         tx.commit();
      }

      sqlite::exec(db, "DELETE FROM registry_info");
      sqlite::Stmt s(db, "INSERT INTO registry_info (schema_checksum, xml_checksum, xml_size, xml_path) VALUES (?, ?, ?, ?)");
      s.bind(1, ( U64 ) get_schema_checksum());
      s.bind(2, ( U64 ) xml_checksum);
      s.bind(3, raw_xml.size());
      s.bind(4, xml_path.generic_string().c_str());
      s.step();
      
      sqlite::vacuum(db);
      sqlite::analyze(db);

      be_short_info("") << "Finished building registry DB." | default_log();
   }

   return db;
}

} // be::bglgen
