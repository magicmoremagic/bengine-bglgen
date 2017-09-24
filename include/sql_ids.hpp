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
   api_id INTEGER,
   PRIMARY KEY (dependent_type_id, nondependent_type_id, api_id)
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
   name TEXT,
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
   op INTEGER NOT NULL,
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
   op INTEGER NOT NULL,
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
   op INTEGER NOT NULL,
   comment TEXT,
   PRIMARY KEY (component_id, command_id, api_id, profile_id)
)
]]},
{'CI_COMMAND_COMPONENT', [[CREATE INDEX command_component_index ON component_commands (command_id, component_id)]]},

{'CV_EXTENSIONS', [[
CREATE VIEW extensions AS
SELECT
   components.id component_id,
   components.name name,
   component_apis.api_id api_id,
   apis.name api,
   components.comment comment
FROM components
LEFT JOIN component_apis ON (components.id = component_apis.component_id) 
LEFT JOIN apis ON (component_apis.api_id = apis.id) 
WHERE components.type = 1
]]},

{'CV_FEATURES', [[
CREATE VIEW features AS
SELECT
   components.id component_id,
   components.name name,
   component_apis.api_id api_id,
   apis.name api,
   components.version_major version_major,
   components.version_minor version_minor,
   components.comment comment
FROM components
LEFT JOIN component_apis ON (components.id = component_apis.component_id)
LEFT JOIN apis ON (component_apis.api_id = apis.id)
WHERE components.type = 0
]]},

{'CV_TYPE_COMPONENTS', [[
CREATE VIEW type_components AS
SELECT
   types.id type_id,
   types.name name,
   component_types.component_id component_id,
   components.name component,
   component_types.api_id api_id,
   apis.name api,
   component_types.profile_id profile_id,
   profiles.name profile,
   component_types.op op,
   components.type component_type,
   components.version_major version_major,
   components.version_minor version_minor
FROM types
INNER JOIN component_types ON (types.id = component_types.type_id)
LEFT JOIN components ON (component_types.type_id = components.id)
LEFT JOIN apis ON (component_types.api_id = apis.id)
LEFT JOIN profiles ON (component_types.profile_id = profiles.id)
]]},

{'CV_ENUM_COMPONENTS', [[
CREATE VIEW enum_components AS
SELECT
   enums.id enum_id,
   enums.name name,
   component_enums.component_id component_id,
   components.name component,
   component_enums.api_id api_id,
   apis.name api,
   component_enums.profile_id profile_id,
   profiles.name profile,
   component_enums.op op,
   components.type component_type,
   components.version_major version_major,
   components.version_minor version_minor
FROM enums
INNER JOIN component_enums ON (enums.id = component_enums.enum_id)
LEFT JOIN components ON (component_enums.component_id = components.id)
LEFT JOIN apis ON (component_enums.api_id = apis.id)
LEFT JOIN profiles ON (component_enums.profile_id = profiles.id)
]]},

{'CV_COMMAND_COMPONENTS', [[
CREATE VIEW command_components AS
SELECT
   commands.id command_id,
   commands.name name,
   component_commands.component_id component_id,
   components.name component,
   component_commands.api_id api_id,
   apis.name api,
   component_commands.profile_id profile_id,
   profiles.name profile,
   component_commands.op op,
   components.type component_type,
   components.version_major version_major,
   components.version_minor version_minor
FROM commands
INNER JOIN component_commands ON (commands.id = component_commands.command_id)
LEFT JOIN components ON (component_commands.component_id = components.id)
LEFT JOIN apis ON (component_commands.api_id = apis.id)
LEFT JOIN profiles ON (component_commands.profile_id = profiles.id)
]]},

{'CV_COMMAND_ALIAS_NAMES', [[
CREATE VIEW command_alias_names AS
SELECT
   cp.id preferred_id,
   cp.name preferred_name,
   ca.id alias_id,
   ca.name alias_name,
   command_aliases.type type
FROM command_aliases
LEFT JOIN commands cp ON (command_aliases.preferred_id = cp.id)
LEFT JOIN commands ca ON (command_aliases.alias_id = ca.id)
]]},

{'CV_API_TYPES', [[
CREATE VIEW api_types AS
SELECT
   types.id type_id,
   types.name name,
   type_declarations.api_id api_id,
   type_declarations.declaration declaration,
   type_declarations.comment comment
FROM types
INNER JOIN type_declarations ON (types.id = type_declarations.type_id)
]]},

{'CV_API_ENUMS', [[
CREATE VIEW api_enums AS
SELECT
   enums.id enum_id,
   enums.name name,
   enum_declarations.api_id api_id,
   enum_aliases.preferred_id,
   enums2.name preferred_name,
   enums.bitmask bitmask,
   enum_declarations.value value,
   enum_declarations.raw_value raw_value,
   enum_declarations.suffix suffix,
   enum_declarations.comment comment
FROM enums
INNER JOIN enum_declarations ON (enums.id = enum_declarations.enum_id)
LEFT JOIN enum_aliases ON (enums.id = enum_aliases.alias_id)
LEFT JOIN enums enums2 ON (enums2.id = enum_aliases.preferred_id)
]]},

{'CV_API_GROUP_ENUMS', [[
CREATE VIEW api_group_enums AS
SELECT
   group_enums.group_id group_id,
   enums.id enum_id,
   enums.name name,
   enum_declarations.api_id api_id,
   enum_aliases.preferred_id,
   enums2.name preferred_name,
   enums.bitmask bitmask,
   enum_declarations.value value,
   enum_declarations.raw_value raw_value,
   enum_declarations.suffix suffix,   
   enum_declarations.comment comment
FROM group_enums
LEFT JOIN enums ON (enums.id = group_enums.enum_id)
INNER JOIN enum_declarations ON (enums.id = enum_declarations.enum_id)
LEFT JOIN enum_aliases ON (enums.id = enum_aliases.alias_id)
LEFT JOIN enums enums2 ON (enums2.id = enum_aliases.preferred_id)
]]},

}) !! 176 */
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
#define BEIDN_BGLGEN_SQL_CT_TYPE_DEPENDENCIES       "CREATE TABLE type_dependencies (\n   dependent_type_id INTEGER NOT NULL,\n   nondependent_type_id INTEGER NOT NULL,\n   api_id INTEGER,\n   PRIMARY KEY (dependent_type_id, nondependent_type_id, api_id)\n)\n"
#define  BEID_BGLGEN_SQL_CT_TYPE_DEPENDENCIES       0x93319E115AE01CE2
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
#define BEIDN_BGLGEN_SQL_CT_COMMAND_SIGNATURE       "CREATE TABLE command_signature (\n   command_id INTEGER NOT NULL,\n   param_index INTEGER NOT NULL, -- 0 for return value, 1 for first parameter, etc.\n   type_id INTEGER,\n   group_id INTEGER,\n   length INTEGER,\n   raw_length TEXT,\n   declaration TEXT NOT NULL,\n   name TEXT,\n   PRIMARY KEY (command_id, param_index)\n)\n"
#define  BEID_BGLGEN_SQL_CT_COMMAND_SIGNATURE       0x20F77156DC3EE5ED
#define BEIDN_BGLGEN_SQL_CT_COMPONENTS              "CREATE TABLE components (\n   id INTEGER PRIMARY KEY AUTOINCREMENT,\n   name TEXT UNIQUE NOT NULL,\n   type INTEGER NOT NULL, -- 0 for feature, 1 for extension\n   version_major INTEGER,\n   version_minor INTEGER,\n   comment TEXT\n)\n"
#define  BEID_BGLGEN_SQL_CT_COMPONENTS              0x14BEC24823290399
#define BEIDN_BGLGEN_SQL_CT_COMPONENT_APIS          "CREATE TABLE component_apis (\n   component_id INTEGER NOT NULL,\n   api_id INTEGER NOT NULL,\n   PRIMARY KEY (component_id, api_id)\n)\n"
#define  BEID_BGLGEN_SQL_CT_COMPONENT_APIS          0xA1CF3450B7CC70D3
#define BEIDN_BGLGEN_SQL_CI_API_COMPONENT           "CREATE INDEX api_component_index ON component_apis (api_id, component_id)"
#define  BEID_BGLGEN_SQL_CI_API_COMPONENT           0xF6BE5392429D104C
#define BEIDN_BGLGEN_SQL_CT_COMPONENT_TYPES         "CREATE TABLE component_types (\n   component_id INTEGER NOT NULL,\n   type_id INTEGER NOT NULL,\n   api_id INTEGER,\n   profile_id INTEGER,\n   op INTEGER NOT NULL,\n   comment TEXT,\n   PRIMARY KEY (component_id, type_id, api_id, profile_id)\n)\n"
#define  BEID_BGLGEN_SQL_CT_COMPONENT_TYPES         0xE88DB85AB7C795DA
#define BEIDN_BGLGEN_SQL_CI_TYPE_COMPONENT          "CREATE INDEX type_component_index ON component_types (type_id, component_id)"
#define  BEID_BGLGEN_SQL_CI_TYPE_COMPONENT          0x2365836397CB3234
#define BEIDN_BGLGEN_SQL_CT_COMPONENT_ENUMS         "CREATE TABLE component_enums (\n   component_id INTEGER NOT NULL,\n   enum_id INTEGER NOT NULL,\n   api_id INTEGER,\n   profile_id INTEGER,\n   op INTEGER NOT NULL,\n   comment TEXT,\n   PRIMARY KEY (component_id, enum_id, api_id, profile_id)\n)\n"
#define  BEID_BGLGEN_SQL_CT_COMPONENT_ENUMS         0xCA82E9B02E5C34B7
#define BEIDN_BGLGEN_SQL_CI_ENUM_COMPONENT          "CREATE INDEX enum_component_index ON component_enums (enum_id, component_id)"
#define  BEID_BGLGEN_SQL_CI_ENUM_COMPONENT          0x714B89465167E781
#define BEIDN_BGLGEN_SQL_CT_COMPONENT_COMMANDS      "CREATE TABLE component_commands (\n   component_id INTEGER NOT NULL,\n   command_id INTEGER NOT NULL,\n   api_id INTEGER,\n   profile_id INTEGER,\n   op INTEGER NOT NULL,\n   comment TEXT,\n   PRIMARY KEY (component_id, command_id, api_id, profile_id)\n)\n"
#define  BEID_BGLGEN_SQL_CT_COMPONENT_COMMANDS      0xE3748C6F68412053
#define BEIDN_BGLGEN_SQL_CI_COMMAND_COMPONENT       "CREATE INDEX command_component_index ON component_commands (command_id, component_id)"
#define  BEID_BGLGEN_SQL_CI_COMMAND_COMPONENT       0x9BFAE048E5AD7E67
#define BEIDN_BGLGEN_SQL_CV_EXTENSIONS              "CREATE VIEW extensions AS\nSELECT\n   components.id component_id,\n   components.name name,\n   component_apis.api_id api_id,\n   apis.name api,\n   components.comment comment\nFROM components\nLEFT JOIN component_apis ON (components.id = component_apis.component_id) \nLEFT JOIN apis ON (component_apis.api_id = apis.id) \nWHERE components.type = 1\n"
#define  BEID_BGLGEN_SQL_CV_EXTENSIONS              0x868CE3255D3FF9E8
#define BEIDN_BGLGEN_SQL_CV_FEATURES                "CREATE VIEW features AS\nSELECT\n   components.id component_id,\n   components.name name,\n   component_apis.api_id api_id,\n   apis.name api,\n   components.version_major version_major,\n   components.version_minor version_minor,\n   components.comment comment\nFROM components\nLEFT JOIN component_apis ON (components.id = component_apis.component_id)\nLEFT JOIN apis ON (component_apis.api_id = apis.id)\nWHERE components.type = 0\n"
#define  BEID_BGLGEN_SQL_CV_FEATURES                0xB449A9F1A7F559D0
#define BEIDN_BGLGEN_SQL_CV_TYPE_COMPONENTS         "CREATE VIEW type_components AS\nSELECT\n   types.id type_id,\n   types.name name,\n   component_types.component_id component_id,\n   components.name component,\n   component_types.api_id api_id,\n   apis.name api,\n   component_types.profile_id profile_id,\n   profiles.name profile,\n   component_types.op op,\n   components.type component_type,\n   components.version_major version_major,\n   components.version_minor version_minor\nFROM types\nINNER JOIN component_types ON (types.id = component_types.type_id)\nLEFT JOIN components ON (component_types.type_id = components.id)\nLEFT JOIN apis ON (component_types.api_id = apis.id)\nLEFT JOIN profiles ON (component_types.profile_id = profiles.id)\n"
#define  BEID_BGLGEN_SQL_CV_TYPE_COMPONENTS         0x7222CF732140268B
#define BEIDN_BGLGEN_SQL_CV_ENUM_COMPONENTS         "CREATE VIEW enum_components AS\nSELECT\n   enums.id enum_id,\n   enums.name name,\n   component_enums.component_id component_id,\n   components.name component,\n   component_enums.api_id api_id,\n   apis.name api,\n   component_enums.profile_id profile_id,\n   profiles.name profile,\n   component_enums.op op,\n   components.type component_type,\n   components.version_major version_major,\n   components.version_minor version_minor\nFROM enums\nINNER JOIN component_enums ON (enums.id = component_enums.enum_id)\nLEFT JOIN components ON (component_enums.component_id = components.id)\nLEFT JOIN apis ON (component_enums.api_id = apis.id)\nLEFT JOIN profiles ON (component_enums.profile_id = profiles.id)\n"
#define  BEID_BGLGEN_SQL_CV_ENUM_COMPONENTS         0x65DC72E07843E020
#define BEIDN_BGLGEN_SQL_CV_COMMAND_COMPONENTS      "CREATE VIEW command_components AS\nSELECT\n   commands.id command_id,\n   commands.name name,\n   component_commands.component_id component_id,\n   components.name component,\n   component_commands.api_id api_id,\n   apis.name api,\n   component_commands.profile_id profile_id,\n   profiles.name profile,\n   component_commands.op op,\n   components.type component_type,\n   components.version_major version_major,\n   components.version_minor version_minor\nFROM commands\nINNER JOIN component_commands ON (commands.id = component_commands.command_id)\nLEFT JOIN components ON (component_commands.component_id = components.id)\nLEFT JOIN apis ON (component_commands.api_id = apis.id)\nLEFT JOIN profiles ON (component_commands.profile_id = profiles.id)\n"
#define  BEID_BGLGEN_SQL_CV_COMMAND_COMPONENTS      0x191E723238577086
#define BEIDN_BGLGEN_SQL_CV_COMMAND_ALIAS_NAMES     "CREATE VIEW command_alias_names AS\nSELECT\n   cp.id preferred_id,\n   cp.name preferred_name,\n   ca.id alias_id,\n   ca.name alias_name,\n   command_aliases.type type\nFROM command_aliases\nLEFT JOIN commands cp ON (command_aliases.preferred_id = cp.id)\nLEFT JOIN commands ca ON (command_aliases.alias_id = ca.id)\n"
#define  BEID_BGLGEN_SQL_CV_COMMAND_ALIAS_NAMES     0x21B3787848581584
#define BEIDN_BGLGEN_SQL_CV_API_TYPES               "CREATE VIEW api_types AS\nSELECT\n   types.id type_id,\n   types.name name,\n   type_declarations.api_id api_id,\n   type_declarations.declaration declaration,\n   type_declarations.comment comment\nFROM types\nINNER JOIN type_declarations ON (types.id = type_declarations.type_id)\n"
#define  BEID_BGLGEN_SQL_CV_API_TYPES               0xBEAD52040F6DAB67
#define BEIDN_BGLGEN_SQL_CV_API_ENUMS               "CREATE VIEW api_enums AS\nSELECT\n   enums.id enum_id,\n   enums.name name,\n   enum_declarations.api_id api_id,\n   enum_aliases.preferred_id,\n   enums2.name preferred_name,\n   enums.bitmask bitmask,\n   enum_declarations.value value,\n   enum_declarations.raw_value raw_value,\n   enum_declarations.suffix suffix,\n   enum_declarations.comment comment\nFROM enums\nINNER JOIN enum_declarations ON (enums.id = enum_declarations.enum_id)\nLEFT JOIN enum_aliases ON (enums.id = enum_aliases.alias_id)\nLEFT JOIN enums enums2 ON (enums2.id = enum_aliases.preferred_id)\n"
#define  BEID_BGLGEN_SQL_CV_API_ENUMS               0xDA5C2E32E27CBB8F
#define BEIDN_BGLGEN_SQL_CV_API_GROUP_ENUMS         "CREATE VIEW api_group_enums AS\nSELECT\n   group_enums.group_id group_id,\n   enums.id enum_id,\n   enums.name name,\n   enum_declarations.api_id api_id,\n   enum_aliases.preferred_id,\n   enums2.name preferred_name,\n   enums.bitmask bitmask,\n   enum_declarations.value value,\n   enum_declarations.raw_value raw_value,\n   enum_declarations.suffix suffix,   \n   enum_declarations.comment comment\nFROM group_enums\nLEFT JOIN enums ON (enums.id = group_enums.enum_id)\nINNER JOIN enum_declarations ON (enums.id = enum_declarations.enum_id)\nLEFT JOIN enum_aliases ON (enums.id = enum_aliases.alias_id)\nLEFT JOIN enums enums2 ON (enums2.id = enum_aliases.preferred_id)\n"
#define  BEID_BGLGEN_SQL_CV_API_GROUP_ENUMS         0x883B503C9DC0BF67

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
extern const Id bglgen_sql_cv_extensions;
extern const Id bglgen_sql_cv_features;
extern const Id bglgen_sql_cv_type_components;
extern const Id bglgen_sql_cv_enum_components;
extern const Id bglgen_sql_cv_command_components;
extern const Id bglgen_sql_cv_command_alias_names;
extern const Id bglgen_sql_cv_api_types;
extern const Id bglgen_sql_cv_api_enums;
extern const Id bglgen_sql_cv_api_group_enums;

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
const Id bglgen_sql_cv_extensions = Id(BEIDN_BGLGEN_SQL_CV_EXTENSIONS);
const Id bglgen_sql_cv_features = Id(BEIDN_BGLGEN_SQL_CV_FEATURES);
const Id bglgen_sql_cv_type_components = Id(BEIDN_BGLGEN_SQL_CV_TYPE_COMPONENTS);
const Id bglgen_sql_cv_enum_components = Id(BEIDN_BGLGEN_SQL_CV_ENUM_COMPONENTS);
const Id bglgen_sql_cv_command_components = Id(BEIDN_BGLGEN_SQL_CV_COMMAND_COMPONENTS);
const Id bglgen_sql_cv_command_alias_names = Id(BEIDN_BGLGEN_SQL_CV_COMMAND_ALIAS_NAMES);
const Id bglgen_sql_cv_api_types = Id(BEIDN_BGLGEN_SQL_CV_API_TYPES);
const Id bglgen_sql_cv_api_enums = Id(BEIDN_BGLGEN_SQL_CV_API_ENUMS);
const Id bglgen_sql_cv_api_group_enums = Id(BEIDN_BGLGEN_SQL_CV_API_GROUP_ENUMS);

} // be::ids
} // be

#endif

/* ######################### END OF GENERATED CODE ######################### */
