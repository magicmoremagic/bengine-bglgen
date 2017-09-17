/*!! include('common/ids', { symbol_prefix = 'BGLGEN_SQL_' }, {

{'CT_REGISTRY_INFO', [[
CREATE TABLE registry_info (
   schema_checksum INTEGER NOT NULL,
   xml_checksum INTEGER NOT NULL,
   xml_size INTEGER NOT NULL,
   xml_path TEXT NOT NULL
)
]]},

{'CT_COMMENTS', [[
CREATE TABLE comments (
   id  INTEGER PRIMARY KEY AUTOINCREMENT,
   comment TEXT NOT NULL
)
]]},

{'CT_APIS', [[
CREATE TABLE apis (
   id INTEGER PRIMARY KEY AUTOINCREMENT,
   name TEXT UNIQUE NOT NULL
)
]]},

{'CT_PROFILES', [[
CREATE TABLE profiles (
   id INTEGER PRIMARY KEY AUTOINCREMENT,
   name TEXT UNIQUE NOT NULL
)
]]},

{'CT_TYPES', [[
CREATE TABLE types (
   id INTEGER PRIMARY KEY AUTOINCREMENT,
   name TEXT UNIQUE NOT NULL
)
]]},

{'CT_TYPE_DECLARATIONS', [[
CREATE TABLE type_declarations (
   type_id INTEGER NOT NULL,
   api_id INTEGER,
   declaration TEXT NOT NULL,
   comment TEXT,
   PRIMARY KEY (type_id, api_id)
)
]]},

{'CT_TYPE_DEPENDENCIES', [[
CREATE TABLE type_dependencies (
   dependent_type_id INTEGER NOT NULL,
   nondependent_type_id INTEGER NOT NULL,
   PRIMARY KEY (dependent_type_id, nondependent_type_id)
)
]]},

{'CT_GROUPS', [[
CREATE TABLE groups (
   id INTEGER PRIMARY KEY AUTOINCREMENT,
   name TEXT UNIQUE NOT NULL
)
]]},

{'CT_GROUP_ENUMS', [[
CREATE TABLE group_enums (
   group_id INTEGER NOT NULL,
   enum_id INTEGER NOT NULL,
   PRIMARY KEY (group_id, enum_id)
)
]]},
{'CI_ENUM_GROUP', [[CREATE INDEX enum_group_index ON group_enums (enum_id, group_id)]]},

{'CT_ENUMS', [[
CREATE TABLE enums (
   id INTEGER PRIMARY KEY AUTOINCREMENT,
   name TEXT UNIQUE NOT NULL,
   bitmask INTEGER NOT NULL
)
]]},

{'CT_ENUM_DECLARATIONS', [[
CREATE TABLE enum_declarations (
   enum_id INTEGER NOT NULL,
   api_id INTEGER,
   value INTEGER,
   raw_value TEXT NOT NULL,
   suffix TEXT,
   comment TEXT,
   PRIMARY KEY (enum_id, api_id)
)
]]},

{'CT_ENUM_ALIASES', [[
CREATE TABLE enum_aliases (
   preferred_id INTEGER NOT NULL,
   alias_id INTEGER NOT NULL,
   PRIMARY KEY (preferred_id, alias_id)
)
]]},
{'CI_ENUM_ALIASES_REVERSE', [[CREATE INDEX enum_aliases_reverse_index ON enum_aliases (alias_id, preferred_id)]]},

{'CT_COMMANDS', [[
CREATE TABLE commands (
   id INTEGER PRIMARY KEY AUTOINCREMENT,
   name TEXT UNIQUE NOT NULL,
   comment TEXT
)
]]},

{'CT_COMMAND_ALIASES', [[
CREATE TABLE command_aliases (
   preferred_id INTEGER NOT NULL,
   alias_id INTEGER NOT NULL,
   type INTEGER NOT NULL, -- 0 for exact alias, 1 for preferred is vectorized version of alias
   PRIMARY KEY (preferred_id, alias_id)
)
]]},
{'CI_COMMAND_ALIASES_REVERSE', [[CREATE INDEX command_aliases_reverse_index ON command_aliases (alias_id, preferred_id)]]},

{'CT_COMMAND_SIGNATURE', [[
CREATE TABLE command_signature (
   command_id INTEGER NOT NULL,
   param_index INTEGER NOT NULL, -- 0 for return value, 1 for first parameter, etc.
   type_id INTEGER,
   group_id INTEGER,
   length INTEGER,
   raw_length TEXT,
   declaration TEXT NOT NULL,
   PRIMARY KEY (command_id, param_index)
)
]]},

{'CT_COMPONENTS', [[
CREATE TABLE components (
   id INTEGER PRIMARY KEY AUTOINCREMENT,
   name TEXT UNIQUE NOT NULL,
   type INTEGER NOT NULL, -- 0 for feature, 1 for extension
   version_major INTEGER,
   version_minor INTEGER,
   comment TEXT
)
]]},

{'CT_COMPONENT_APIS', [[
CREATE TABLE component_apis (
   component_id INTEGER NOT NULL,
   api_id INTEGER NOT NULL,
   PRIMARY KEY (component_id, api_id)
)
]]},
{'CI_API_COMPONENT', [[CREATE INDEX api_component_index ON component_apis (api_id, component_id)]]},

{'CT_COMPONENT_TYPES', [[
CREATE TABLE component_types (
   component_id INTEGER NOT NULL,
   type_id INTEGER NOT NULL,
   api_id INTEGER,
   profile_id INTEGER,
   action INTEGER NOT NULL,
   comment TEXT,
   PRIMARY KEY (component_id, type_id, api_id, profile_id)
)
]]},
{'CI_TYPE_COMPONENT', [[CREATE INDEX type_component_index ON component_types (type_id, component_id)]]},

{'CT_COMPONENT_ENUMS', [[
CREATE TABLE component_enums (
   component_id INTEGER NOT NULL,
   enum_id INTEGER NOT NULL,
   api_id INTEGER,
   profile_id INTEGER,
   action INTEGER NOT NULL,
   comment TEXT,
   PRIMARY KEY (component_id, enum_id, api_id, profile_id)
)
]]},
{'CI_ENUM_COMPONENT', [[CREATE INDEX enum_component_index ON component_enums (enum_id, component_id)]]},

{'CT_COMPONENT_COMMANDS', [[
CREATE TABLE component_commands (
   component_id INTEGER NOT NULL,
   command_id INTEGER NOT NULL,
   api_id INTEGER,
   profile_id INTEGER,
   action INTEGER NOT NULL,
   comment TEXT,
   PRIMARY KEY (component_id, command_id, api_id, profile_id)
)
]]},
{'CI_COMMAND_COMPONENT', [[CREATE INDEX command_component_index ON component_commands (command_id, component_id)]]}

}) !! 140 */
/* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */
#pragma once
#ifndef BE_BGLGEN_SQL_IDS_HPP_
#define BE_BGLGEN_SQL_IDS_HPP_

#include <be/core/id.hpp>

#define BEIDN_BGLGEN_SQL_CT_REGISTRY_INFO           "CREATE TABLE registry_info (\n   schema_checksum INTEGER NOT NULL,\n   xml_checksum INTEGER NOT NULL,\n   xml_size INTEGER NOT NULL,\n   xml_path TEXT NOT NULL\n)\n"
#define  BEID_BGLGEN_SQL_CT_REGISTRY_INFO           0x8074BDF27AD4EBB1
#define BEIDN_BGLGEN_SQL_CT_COMMENTS                "CREATE TABLE comments (\n   id  INTEGER PRIMARY KEY AUTOINCREMENT,\n   comment TEXT NOT NULL\n)\n"
#define  BEID_BGLGEN_SQL_CT_COMMENTS                0xC2173BCC4A59E716
#define BEIDN_BGLGEN_SQL_CT_APIS                    "CREATE TABLE apis (\n   id INTEGER PRIMARY KEY AUTOINCREMENT,\n   name TEXT UNIQUE NOT NULL\n)\n"
#define  BEID_BGLGEN_SQL_CT_APIS                    0x3AF57C3BA9E28C3A
#define BEIDN_BGLGEN_SQL_CT_PROFILES                "CREATE TABLE profiles (\n   id INTEGER PRIMARY KEY AUTOINCREMENT,\n   name TEXT UNIQUE NOT NULL\n)\n"
#define  BEID_BGLGEN_SQL_CT_PROFILES                0x95FDF5BDEF44F5F3
#define BEIDN_BGLGEN_SQL_CT_TYPES                   "CREATE TABLE types (\n   id INTEGER PRIMARY KEY AUTOINCREMENT,\n   name TEXT UNIQUE NOT NULL\n)\n"
#define  BEID_BGLGEN_SQL_CT_TYPES                   0x35899F08C2BA2188
#define BEIDN_BGLGEN_SQL_CT_TYPE_DECLARATIONS       "CREATE TABLE type_declarations (\n   type_id INTEGER NOT NULL,\n   api_id INTEGER,\n   declaration TEXT NOT NULL,\n   comment TEXT,\n   PRIMARY KEY (type_id, api_id)\n)\n"
#define  BEID_BGLGEN_SQL_CT_TYPE_DECLARATIONS       0x25FD017DF55535A3
#define BEIDN_BGLGEN_SQL_CT_TYPE_DEPENDENCIES       "CREATE TABLE type_dependencies (\n   dependent_type_id INTEGER NOT NULL,\n   nondependent_type_id INTEGER NOT NULL,\n   PRIMARY KEY (dependent_type_id, nondependent_type_id)\n)\n"
#define  BEID_BGLGEN_SQL_CT_TYPE_DEPENDENCIES       0x6BB3BE84DDB50DB2
#define BEIDN_BGLGEN_SQL_CT_GROUPS                  "CREATE TABLE groups (\n   id INTEGER PRIMARY KEY AUTOINCREMENT,\n   name TEXT UNIQUE NOT NULL\n)\n"
#define  BEID_BGLGEN_SQL_CT_GROUPS                  0xC1D2E9768AD7841D
#define BEIDN_BGLGEN_SQL_CT_GROUP_ENUMS             "CREATE TABLE group_enums (\n   group_id INTEGER NOT NULL,\n   enum_id INTEGER NOT NULL,\n   PRIMARY KEY (group_id, enum_id)\n)\n"
#define  BEID_BGLGEN_SQL_CT_GROUP_ENUMS             0x6AA83E32FA7AB710
#define BEIDN_BGLGEN_SQL_CI_ENUM_GROUP              "CREATE INDEX enum_group_index ON group_enums (enum_id, group_id)"
#define  BEID_BGLGEN_SQL_CI_ENUM_GROUP              0x273C25C958064C5D
#define BEIDN_BGLGEN_SQL_CT_ENUMS                   "CREATE TABLE enums (\n   id INTEGER PRIMARY KEY AUTOINCREMENT,\n   name TEXT UNIQUE NOT NULL,\n   bitmask INTEGER NOT NULL\n)\n"
#define  BEID_BGLGEN_SQL_CT_ENUMS                   0x7F7B50F3F9988EF0
#define BEIDN_BGLGEN_SQL_CT_ENUM_DECLARATIONS       "CREATE TABLE enum_declarations (\n   enum_id INTEGER NOT NULL,\n   api_id INTEGER,\n   value INTEGER,\n   raw_value TEXT NOT NULL,\n   suffix TEXT,\n   comment TEXT,\n   PRIMARY KEY (enum_id, api_id)\n)\n"
#define  BEID_BGLGEN_SQL_CT_ENUM_DECLARATIONS       0x24C019FAE550BE8D
#define BEIDN_BGLGEN_SQL_CT_ENUM_ALIASES            "CREATE TABLE enum_aliases (\n   preferred_id INTEGER NOT NULL,\n   alias_id INTEGER NOT NULL,\n   PRIMARY KEY (preferred_id, alias_id)\n)\n"
#define  BEID_BGLGEN_SQL_CT_ENUM_ALIASES            0x7057ACDB933491DE
#define BEIDN_BGLGEN_SQL_CI_ENUM_ALIASES_REVERSE    "CREATE INDEX enum_aliases_reverse_index ON enum_aliases (alias_id, preferred_id)"
#define  BEID_BGLGEN_SQL_CI_ENUM_ALIASES_REVERSE    0x6F91D6955A524520
#define BEIDN_BGLGEN_SQL_CT_COMMANDS                "CREATE TABLE commands (\n   id INTEGER PRIMARY KEY AUTOINCREMENT,\n   name TEXT UNIQUE NOT NULL,\n   comment TEXT\n)\n"
#define  BEID_BGLGEN_SQL_CT_COMMANDS                0xC38A15598A9F4431
#define BEIDN_BGLGEN_SQL_CT_COMMAND_ALIASES         "CREATE TABLE command_aliases (\n   preferred_id INTEGER NOT NULL,\n   alias_id INTEGER NOT NULL,\n   type INTEGER NOT NULL, -- 0 for exact alias, 1 for preferred is vectorized version of alias\n   PRIMARY KEY (preferred_id, alias_id)\n)\n"
#define  BEID_BGLGEN_SQL_CT_COMMAND_ALIASES         0xA1ECF00BD23CE187
#define BEIDN_BGLGEN_SQL_CI_COMMAND_ALIASES_REVERSE "CREATE INDEX command_aliases_reverse_index ON command_aliases (alias_id, preferred_id)"
#define  BEID_BGLGEN_SQL_CI_COMMAND_ALIASES_REVERSE 0xBC8B8294CD1A0246
#define BEIDN_BGLGEN_SQL_CT_COMMAND_SIGNATURE       "CREATE TABLE command_signature (\n   command_id INTEGER NOT NULL,\n   param_index INTEGER NOT NULL, -- 0 for return value, 1 for first parameter, etc.\n   type_id INTEGER,\n   group_id INTEGER,\n   length INTEGER,\n   raw_length TEXT,\n   declaration TEXT NOT NULL,\n   PRIMARY KEY (command_id, param_index)\n)\n"
#define  BEID_BGLGEN_SQL_CT_COMMAND_SIGNATURE       0xB9847541DA5D3FA5
#define BEIDN_BGLGEN_SQL_CT_COMPONENTS              "CREATE TABLE components (\n   id INTEGER PRIMARY KEY AUTOINCREMENT,\n   name TEXT UNIQUE NOT NULL,\n   type INTEGER NOT NULL, -- 0 for feature, 1 for extension\n   version_major INTEGER,\n   version_minor INTEGER,\n   comment TEXT\n)\n"
#define  BEID_BGLGEN_SQL_CT_COMPONENTS              0x14BEC24823290399
#define BEIDN_BGLGEN_SQL_CT_COMPONENT_APIS          "CREATE TABLE component_apis (\n   component_id INTEGER NOT NULL,\n   api_id INTEGER NOT NULL,\n   PRIMARY KEY (component_id, api_id)\n)\n"
#define  BEID_BGLGEN_SQL_CT_COMPONENT_APIS          0xA1CF3450B7CC70D3
#define BEIDN_BGLGEN_SQL_CI_API_COMPONENT           "CREATE INDEX api_component_index ON component_apis (api_id, component_id)"
#define  BEID_BGLGEN_SQL_CI_API_COMPONENT           0xF6BE5392429D104C
#define BEIDN_BGLGEN_SQL_CT_COMPONENT_TYPES         "CREATE TABLE component_types (\n   component_id INTEGER NOT NULL,\n   type_id INTEGER NOT NULL,\n   api_id INTEGER,\n   profile_id INTEGER,\n   action INTEGER NOT NULL,\n   comment TEXT,\n   PRIMARY KEY (component_id, type_id, api_id, profile_id)\n)\n"
#define  BEID_BGLGEN_SQL_CT_COMPONENT_TYPES         0x30CACF25C2C78C25
#define BEIDN_BGLGEN_SQL_CI_TYPE_COMPONENT          "CREATE INDEX type_component_index ON component_types (type_id, component_id)"
#define  BEID_BGLGEN_SQL_CI_TYPE_COMPONENT          0x2365836397CB3234
#define BEIDN_BGLGEN_SQL_CT_COMPONENT_ENUMS         "CREATE TABLE component_enums (\n   component_id INTEGER NOT NULL,\n   enum_id INTEGER NOT NULL,\n   api_id INTEGER,\n   profile_id INTEGER,\n   action INTEGER NOT NULL,\n   comment TEXT,\n   PRIMARY KEY (component_id, enum_id, api_id, profile_id)\n)\n"
#define  BEID_BGLGEN_SQL_CT_COMPONENT_ENUMS         0xC1172C33168DA464
#define BEIDN_BGLGEN_SQL_CI_ENUM_COMPONENT          "CREATE INDEX enum_component_index ON component_enums (enum_id, component_id)"
#define  BEID_BGLGEN_SQL_CI_ENUM_COMPONENT          0x714B89465167E781
#define BEIDN_BGLGEN_SQL_CT_COMPONENT_COMMANDS      "CREATE TABLE component_commands (\n   component_id INTEGER NOT NULL,\n   command_id INTEGER NOT NULL,\n   api_id INTEGER,\n   profile_id INTEGER,\n   action INTEGER NOT NULL,\n   comment TEXT,\n   PRIMARY KEY (component_id, command_id, api_id, profile_id)\n)\n"
#define  BEID_BGLGEN_SQL_CT_COMPONENT_COMMANDS      0xDE98B37C56D1100E
#define BEIDN_BGLGEN_SQL_CI_COMMAND_COMPONENT       "CREATE INDEX command_component_index ON component_commands (command_id, component_id)"
#define  BEID_BGLGEN_SQL_CI_COMMAND_COMPONENT       0x9BFAE048E5AD7E67

#ifdef BE_ID_EXTERNS

namespace be {
namespace ids {

extern const Id bglgen_sql_ct_registry_info;
extern const Id bglgen_sql_ct_comments;
extern const Id bglgen_sql_ct_apis;
extern const Id bglgen_sql_ct_profiles;
extern const Id bglgen_sql_ct_types;
extern const Id bglgen_sql_ct_type_declarations;
extern const Id bglgen_sql_ct_type_dependencies;
extern const Id bglgen_sql_ct_groups;
extern const Id bglgen_sql_ct_group_enums;
extern const Id bglgen_sql_ci_enum_group;
extern const Id bglgen_sql_ct_enums;
extern const Id bglgen_sql_ct_enum_declarations;
extern const Id bglgen_sql_ct_enum_aliases;
extern const Id bglgen_sql_ci_enum_aliases_reverse;
extern const Id bglgen_sql_ct_commands;
extern const Id bglgen_sql_ct_command_aliases;
extern const Id bglgen_sql_ci_command_aliases_reverse;
extern const Id bglgen_sql_ct_command_signature;
extern const Id bglgen_sql_ct_components;
extern const Id bglgen_sql_ct_component_apis;
extern const Id bglgen_sql_ci_api_component;
extern const Id bglgen_sql_ct_component_types;
extern const Id bglgen_sql_ci_type_component;
extern const Id bglgen_sql_ct_component_enums;
extern const Id bglgen_sql_ci_enum_component;
extern const Id bglgen_sql_ct_component_commands;
extern const Id bglgen_sql_ci_command_component;

} // be::ids
} // be

#endif
#endif
#ifdef BE_ID_STATICS

namespace be {
namespace ids {

const Id bglgen_sql_ct_registry_info = Id(BEIDN_BGLGEN_SQL_CT_REGISTRY_INFO);
const Id bglgen_sql_ct_comments = Id(BEIDN_BGLGEN_SQL_CT_COMMENTS);
const Id bglgen_sql_ct_apis = Id(BEIDN_BGLGEN_SQL_CT_APIS);
const Id bglgen_sql_ct_profiles = Id(BEIDN_BGLGEN_SQL_CT_PROFILES);
const Id bglgen_sql_ct_types = Id(BEIDN_BGLGEN_SQL_CT_TYPES);
const Id bglgen_sql_ct_type_declarations = Id(BEIDN_BGLGEN_SQL_CT_TYPE_DECLARATIONS);
const Id bglgen_sql_ct_type_dependencies = Id(BEIDN_BGLGEN_SQL_CT_TYPE_DEPENDENCIES);
const Id bglgen_sql_ct_groups = Id(BEIDN_BGLGEN_SQL_CT_GROUPS);
const Id bglgen_sql_ct_group_enums = Id(BEIDN_BGLGEN_SQL_CT_GROUP_ENUMS);
const Id bglgen_sql_ci_enum_group = Id(BEIDN_BGLGEN_SQL_CI_ENUM_GROUP);
const Id bglgen_sql_ct_enums = Id(BEIDN_BGLGEN_SQL_CT_ENUMS);
const Id bglgen_sql_ct_enum_declarations = Id(BEIDN_BGLGEN_SQL_CT_ENUM_DECLARATIONS);
const Id bglgen_sql_ct_enum_aliases = Id(BEIDN_BGLGEN_SQL_CT_ENUM_ALIASES);
const Id bglgen_sql_ci_enum_aliases_reverse = Id(BEIDN_BGLGEN_SQL_CI_ENUM_ALIASES_REVERSE);
const Id bglgen_sql_ct_commands = Id(BEIDN_BGLGEN_SQL_CT_COMMANDS);
const Id bglgen_sql_ct_command_aliases = Id(BEIDN_BGLGEN_SQL_CT_COMMAND_ALIASES);
const Id bglgen_sql_ci_command_aliases_reverse = Id(BEIDN_BGLGEN_SQL_CI_COMMAND_ALIASES_REVERSE);
const Id bglgen_sql_ct_command_signature = Id(BEIDN_BGLGEN_SQL_CT_COMMAND_SIGNATURE);
const Id bglgen_sql_ct_components = Id(BEIDN_BGLGEN_SQL_CT_COMPONENTS);
const Id bglgen_sql_ct_component_apis = Id(BEIDN_BGLGEN_SQL_CT_COMPONENT_APIS);
const Id bglgen_sql_ci_api_component = Id(BEIDN_BGLGEN_SQL_CI_API_COMPONENT);
const Id bglgen_sql_ct_component_types = Id(BEIDN_BGLGEN_SQL_CT_COMPONENT_TYPES);
const Id bglgen_sql_ci_type_component = Id(BEIDN_BGLGEN_SQL_CI_TYPE_COMPONENT);
const Id bglgen_sql_ct_component_enums = Id(BEIDN_BGLGEN_SQL_CT_COMPONENT_ENUMS);
const Id bglgen_sql_ci_enum_component = Id(BEIDN_BGLGEN_SQL_CI_ENUM_COMPONENT);
const Id bglgen_sql_ct_component_commands = Id(BEIDN_BGLGEN_SQL_CT_COMPONENT_COMMANDS);
const Id bglgen_sql_ci_command_component = Id(BEIDN_BGLGEN_SQL_CI_COMMAND_COMPONENT);

} // be::ids
} // be

#endif

/* ######################### END OF GENERATED CODE ######################### */
