local table = table
local debug = debug
local string = string
local type = type
local select = select
local ipairs = ipairs
local dofile = dofile
local getmetatable = getmetatable
local setmetatable = setmetatable
local q = gl_registry.query

local fs = require('be.fs')
local util = require('be.util')
local blt = require('be.blt')

do -- strict.lua
   -- checks uses of undeclared global variables
   -- All global variables must be 'declared' through a regular assignment
   -- (even assigning nil will do) in a main chunk before being used
   -- anywhere or assigned to inside a function.
   local mt = getmetatable(_G)
   if mt == nil then
      mt = {}
      setmetatable(_G, mt)
   end

   __STRICT = true
   mt.__declared = {}

   mt.__newindex = function (t, n, v)
      if __STRICT and not mt.__declared[n] then
         local w = debug.getinfo(2, "S").what
         if w ~= "main" and w ~= "C" then
            error("assign to undeclared variable '"..n.."'", 2)
         end
         mt.__declared[n] = true
      end
      rawset(t, n, v)
   end

   mt.__index = function (t, n)
      if __STRICT and not mt.__declared[n] and debug.getinfo(2, "S").what ~= "C" then
         error("variable '"..n.."' is not declared", 2)
      end
      return rawget(t, n)
   end

   function global(...)
      for _, v in ipairs{...} do mt.__declared[v] = true end
   end

end

local ignored_ = { }
ignored = ignored_

local auto_ = { }
auto = auto_

default_registry_path = nil
default_registry_db_path = nil
default_output_path = nil
default_source_extensions = nil
default_search_paths = { }

config = {
   api = 'gl',
   target_version = '3.3',
   profile = 'compatibility',
   auto_related_features = true,
   auto_related_extensions = true,
   skip_codegen = false,
   template = 'bgl_default',
   features = { },
   extensions = { },
   enums = { },
   commands = { },
   symbols = { }
}

found_functions = { }
found_constants = { }

reserved_identifiers = {
   alignas = true,   alignof = true,       ['and'] = true,       and_eq = true,
   asm = true,       atomic_cancel = true, atomic_commit = true, atomic_noexcept = true,
   auto = true,      bitand = true,        bitor = true,         bool = true,
   ['break'] = true, case = true,          catch = true,         char = true,
   char16_t = true,  char32_t = true,      class = true,         compl = true,
   concept = true,   const = true,         constexpr = true,     const_cast = true,
   continue = true,  decltype = true,      default = true,       delete = true,
   ['do'] = true,    double = true,        dynamic_cast = true,  ['else'] = true,
   enum = true,      explicit = true,      export = true,        extern = true,
   ['false'] = true, float = true,         ['for'] = true,       friend = true,
   ['goto'] = true,  ['if'] = true,        import = true,        inline = true,
   int = true,       long = true,          module = true,        mutable = true,
   namespace = true, new = true,           noexcept = true,      ['not'] = true,
   not_eq = true,    nullptr = true,       operator = true,      ['or'] = true,
   or_eq = true,     private = true,       protected = true,     public = true,
   register = true,  reinterpret_cast = true,   requires = true, ['return'] = true,
   short = true,     signed = true,        sizeof = true,        static = true,
   static_assert = true, static_cast = true, struct = true,      switch = true,
   synchronized = true,  template = true,  this = true,          thread_local = true,
   throw = true,     ['true'] = true,      try = true,           typedef = true,
   typeid = true,    typename = true,      union = true,         unsigned = true,
   using = true,     virtual = true,       void = true,          volatile = true,
   wchar_t = true,   ['while'] = true,     xor = true,           xor_eq = true,
}


function dependency () end -- depfile not supported currently

indent_size = 3
indent_char = ' '
bglgenrc_path = nil
base_indent = nil
root_dir = nil

function trim_trailing_ws (str)
   return str:gsub('[ \t]+(\r?\n)', '%1'):gsub('[ \t]+$', '')
end

function postprocess (str)
   return trim_trailing_ws(str)
end

do -- indent
   local current_indent = 0

   function get_indent ()
      local retval = ''
      if base_indent ~= nil and base_indent ~= '' then
         retval = base_indent
      end
      return retval .. string.rep(indent_char, current_indent * indent_size)
   end

   function write_indent ()
      if base_indent ~= nil and base_indent ~= '' then
         write(base_indent)
      end
      local indent = string.rep(indent_char, current_indent * indent_size)
      if indent ~= '' then
         write(indent)
      end
   end

   function reset_indent ()
      current_indent = 0
   end

   function indent (count)
      if count == nil then count = 1 end
      current_indent = current_indent + count
   end

   function unindent (count)
      if count == nil then count = 1 end
      current_indent = current_indent - count
   end

   function set_indent (count)
      current_indent = count
   end

end

function indent_newlines (str)
   str = str:gsub('\n', '\n' .. get_indent())
   return str
end

do -- write
   local sections = { }
   local out = nil
   local n

   function section (s)
      if not sections[s] then
	     sections[s] = { }
	  end

	  out = sections[s]
	  n = #out + 1
   end

   local function init ()
      reset_indent()
      section(1)
   end

   function nl ()
      if out == nil then
         init()
      end
      out[n] = '\n'
      n = n + 1
      write_indent()
   end

   function write (...)
      if out == nil then
         init()
      end
      for i = 1, select('#', ...) do
         local x = select(i, ...)
         if x ~= nil then
            out[n] = x
            n = n + 1
         end
      end
   end

   function writeln (...)
      if out == nil then
         init()
      end
      for i = 1, select('#', ...) do
         local x = select(i, ...)
         if x ~= nil then
            out[n] = x
            n = n + 1
         end
      end
      nl()
   end

   function write_lines (...)
      if out == nil then
         init()
      end
      for i = 1, select('#', ...) do
         local x = select(i, ...)
         if x ~= nil then
            out[n] = x
            n = n + 1
         end
         nl()
      end
   end

   function reset ()
      local section_table = { }
      for i in pairs(sections) do
         section_table[#section_table + 1] = i
      end
      table.sort(section_table)

      local section_strings = { }
      for i = 1, #section_table do
         section_strings[#section_strings + 1] = table.concat(sections[section_table[i]])
      end

      local str = table.concat(section_strings)
      sections = { }
      out = nil

      if type(postprocess) == 'function' then
         str = postprocess(str)
      end

      return str
   end
end

require_load = util.require_load

get_template = blt.get_template
register_template_dir = blt.register_template_dir
register_template_file = blt.register_template_file
register_template_string = blt.register_template_string

pgsub = blt.pgsub
explode = blt.explode
pad = blt.pad
rpad = blt.rpad
lpad = blt.lpad

function template (template_name, ...)
   return blt.get_template(template_name)(...)
end

function write_template (template_name, ...)
   write(indent_newlines(template(template_name, ...)))
end

------------------------------------------------------------------------------
-- Takes an optional table, appends a value to it, and returns it.  If the
-- table is nil, a new table is created.
local function tappend (table, value)
   table = table or { }
   table[#table + 1] = value
   return table
end

------------------------------------------------------------------------------
local function make_sorted (set, cmp)
   local t = { }
   local n = 0
   for k in pairs(set) do
      n = n + 1
      t[n] = { id = k }
   end
   table.sort(t, cmp or function (a, b) return a.id < b.id end)
   return t
end

------------------------------------------------------------------------------
-- Adds the references provided to the be.log parameter tables and returns them.
-- If the parameter tables are nil, they will be created.
-- A ref object is one that looks like { path = '...', line = 1 }
local function log_refs (refs, attrs, vals)
   attrs = attrs or { }
   vals = vals or { }
   for i = 1, #refs do
      local ref = refs[i]
      local a = #attrs+1
      attrs[a] = 'Reference'
      vals[a] = ref.path .. ' : ' .. ref.line
   end
   return attrs, vals
end

------------------------------------------------------------------------------
-- Adds an attribute to the provided be.log parameter tables and returns them
-- If the parameter tables are nil, they will be created.
-- `data` can be either a primitive or a table-list.
local function log_attr (attr, data, attrs, vals)
   if data ~= nil then
      attrs = attrs or { }
      vals = vals or { }
      local i = #attrs + 1
      attrs[i] = attr
      if type(data) == 'table' then
         vals[i] = table.concat(data, ', ')
      else
         vals[i] = data
      end
   end
   return attrs, vals
end

------------------------------------------------------------------------------
-- Returns true iff major1.minor1 <= major2.minor2
local function version_le (major1, minor1, major2, minor2)
   if major1 < major2 then
      return true
   elseif major1 == major2 and minor1 <= minor2 then
      return true
   end
   return false
end

------------------------------------------------------------------------------
-- Determine if the feature defined by `name`, `major`, `minor` is allowed to be used in the context of `check`
local function check_feature (ctx, check, name, major, minor)
   local cfg = ctx.cfg.features[name]

   if cfg == nil or cfg == auto_ then
      if version_le(major, minor, ctx.version_major, ctx.version_minor) then
         return true
      elseif check ~= nil then
		 if check == name then return true end
         local maj, min = string.match(check, '(%d+)%.(%d+)')
		 if maj and min then
			return version_le(major, minor, tonumber(maj), tonumber(min))
		 end
		 return false
      end
   elseif cfg == true then
      return true
   elseif cfg ~= false then
      local t = type(cfg)
      if t == 'string' then
         return check == cfg
      elseif t == 'table' then
         for i = 1, #cfg do
            if check == cfg[i] then
               return true
            end
         end
      end

   end
   return false
end

------------------------------------------------------------------------------
-- Determine if the extension defined by `name` is allowed to be used in the context of `check`
local function check_extension (ctx, check, name)
   local cfg = ctx.cfg.extensions[name]

   if cfg == nil or cfg == auto_ then
      return check == name
   elseif cfg == true then
      return true
   elseif cfg ~= false then
      local t = type(cfg)
      if t == 'string' then
         return check == cfg
      elseif t == 'table' then
         for i = 1, #cfg do
            if check == cfg[i] then return true end
         end
      end
   end
   return false
end

------------------------------------------------------------------------------
local function handle_feature (ctx, constant)
   local id = q([[
      SELECT component_id FROM features
      WHERE name = ? AND (api_id = ? OR api_id IS NULL)
      ORDER BY api_id DESC LIMIT 1
      ]], constant.name, ctx.api_id)

   if not id then return end

   be.log.short_verbose('Processing Feature ' .. constant.name .. ' (' .. id .. ')')

   local cfg = ctx.cfg.features[constant.name]
   if cfg == nil or cfg == auto_ then
      -- normal
      ctx.features[id] = true

   elseif cfg == false then
      -- disallowed unconditionally (error)
      be.log.warning('Feature ' .. constant.name .. ' is not allowed!',
         log_refs(constant.refs))

   elseif cfg == ignored_ then
      -- ignored (no codegen)

   elseif cfg == true then
      -- allowed unconditionally
      ctx.features[id] = true

   else
      local t = type(cfg)
      if t == 'string' or t == 'table' then
         -- require specific check string
         local badrefs
         for i = 1, #constant.refs do
            local ref = constant.refs[i]
            local ok
            if t == 'string' then
               if ref.check == cfg then ok = true end
            else
               for c = 1, #cfg do
                  if ref.check == cfg[c] then ok = true end
               end
            end
            if not ok then badrefs = tappend(badrefs, ref) end
         end
         if badrefs then
            be.log.warning('Feature ' .. constant.name .. ' is not allowed unchecked!',
               log_refs(badrefs, log_attr('Allowed Checks', cfg)))
         else
            ctx.features[id] = true
         end
      else
         be.log.warning('Feature ' .. constant.name .. ' configuration is invalid; ignoring symbol.',
            log_refs(constant.refs))
      end
   end
   return true
end

------------------------------------------------------------------------------
local function handle_extension (ctx, constant)
   local id = q([[
      SELECT component_id FROM extensions
      WHERE name = ? AND (api_id = ? OR api_id IS NULL)
      ORDER BY api_id DESC LIMIT 1
      ]], constant.name, ctx.api_id)

   if not id then return end

   be.log.short_verbose('Processing Extension ' .. constant.name .. ' (' .. id .. ')')

   local cfg = ctx.cfg.extensions[constant.name]
   if cfg == nil or cfg == auto_ then
      -- normal
      ctx.extensions[id] = true

   elseif cfg == false then
      -- disallowed unconditionally (error)
      be.log.warning('Extension ' .. constant.name .. ' is not allowed!',
         log_refs(constant.refs))

   elseif cfg == ignored_ then
      -- ignored (no codegen)

   elseif cfg == true then
      -- allowed unconditionally
      ctx.extensions[id] = true

   else
      local t = type(cfg)
      if t == 'string' or t == 'table' then
         -- require specific check string
         local badrefs
         for i = 1, #constant.refs do
            local ref = constant.refs[i]
            local ok
            if t == 'string' then
               if ref.check == cfg then ok = true end
            else
               for c = 1, #cfg do
                  if ref.check == cfg[c] then ok = true end
               end
            end
            if not ok then badrefs = tappend(badrefs, ref) end
         end
         if badrefs then
            be.log.warning('Extension ' .. constant.name .. ' is not allowed unchecked!',
               log_refs(badrefs, log_attr('Allowed Checks', cfg)))
         else
            ctx.extensions[id] = true
         end
      else
         be.log.warning('Extension ' .. constant.name .. ' configuration is invalid; ignoring symbol.',
            log_refs(constant.refs))
      end
   end
   return true
end

------------------------------------------------------------------------------
local function handle_enum_auto (ctx, constant, id)
   local defined_by
   local deprecated_by
   q([[
      SELECT component_id, component, op, component_type, version_major, version_minor
      FROM enum_components WHERE enum_id = ? AND (api_id = ? OR api_id IS NULL) AND (profile_id = ? OR profile_id IS NULL)
      ORDER BY component_type ASC, version_major ASC, version_minor ASC, api_id ASC, profile_id ASC
      ]], { id, ctx.api_id, ctx.profile_id },
      function (component_id, component, op, component_type, version_major, version_minor)
         if op == 1 then
            defined_by = tappend(defined_by, component)
         else
            deprecated_by = tappend(deprecated_by, component)
         end

         if component_type == 0 then
            if ctx.cfg.auto_related_features then
               ctx.features[component_id] = auto_
            end
            for i = 1, #constant.refs do
               local ref = constant.refs[i]
               if check_feature(ctx, ref.check, component, version_major, version_minor) then
                  ref.ok = op == 1
               end
            end
         else
            if ctx.cfg.auto_related_extensions then
               ctx.extensions[component_id] = auto_
            end
            for i = 1, #constant.refs do
               local ref = constant.refs[i]
               if check_extension(ctx, ref.check, component) then
                  ref.ok = op == 1
               end
            end
         end
      end)

   local badrefs
   local deprecatedrefs
   for i = 1, #constant.refs do
      local ref = constant.refs[i]
      if ref.ok == nil then
         badrefs = tappend(badrefs, ref)
      elseif ref.ok == false then
         deprecatedrefs = tappend(deprecatedrefs, ref)
      elseif ref.ok == true then
         ctx.enums[id] = true
      end
   end

   if deprecatedrefs then
      be.log.warning('Enum ' .. constant.name .. ' is deprecated!',
         log_refs(deprecatedrefs, log_attr('Deprecated By', deprecated_by, log_attr('Defined By', defined_by))))
   end

   if badrefs then
      be.log.warning('Enum ' .. constant.name .. ' is not defined by any allowed features or extensions!',
         log_refs(badrefs, log_attr('Deprecated By', deprecated_by, log_attr('Defined By', defined_by))))
   end
end

------------------------------------------------------------------------------
local function handle_enum (ctx, constant)
   local id = q([[
      SELECT enum_id FROM api_enums
      WHERE name = ? AND (api_id = ? OR api_id IS NULL)
      ORDER BY api_id DESC LIMIT 1
      ]], constant.name, ctx.api_id)

   if not id then return end

   be.log.short_verbose('Processing Enum ' .. constant.name .. ' (' .. id .. ')')

   local cfg = ctx.cfg.enums[constant.name]
   if cfg == nil or cfg == auto_ then
      -- normal
      handle_enum_auto(ctx, constant, id)

   elseif cfg == false then
      -- disallowed unconditionally (error)
      be.log.warning('Enum ' .. constant.name .. ' is not allowed!',
         log_refs(constant.refs))

   elseif cfg == ignored_ then
      -- ignored (no codegen)

   elseif cfg == true then
      -- allowed unconditionally
      ctx.enums[id] = true

   else
      local t = type(cfg)
      if t == 'string' or t == 'table' then
         -- require specific check string
         local badrefs
         for i = 1, #constant.refs do
            local ref = constant.refs[i]
            local ok
            if t == 'string' then
               if ref.check == cfg then ok = true end
            else
               for c = 1, #cfg do
                  if ref.check == cfg[c] then ok = true end
               end
            end
            if not ok then badrefs = tappend(badrefs, ref) end
         end
         if badrefs then
            be.log.warning('Enum ' .. constant.name .. ' is not allowed unchecked!',
               log_refs(badrefs, log_attr('Allowed Checks', cfg)))
         else
            ctx.enums[id] = true
         end
      else
         be.log.warning('Enum ' .. constant.name .. ' configuration is invalid; ignoring symbol.',
            log_refs(constant.refs))
      end
   end
   return true
end

------------------------------------------------------------------------------
local function handle_command_auto (ctx, func, id)
   local defined_by
   local deprecated_by
   q([[
      SELECT component_id, component, op, component_type, version_major, version_minor
      FROM command_components WHERE command_id = ? AND (api_id = ? OR api_id IS NULL) AND (profile_id = ? OR profile_id IS NULL)
      ORDER BY component_type ASC, version_major ASC, version_minor ASC, api_id ASC, profile_id ASC
      ]], { id, ctx.api_id, ctx.profile_id },
      function (component_id, component, op, component_type, version_major, version_minor)
         if op == 1 then
            defined_by = tappend(defined_by, component)
         else
            deprecated_by = tappend(deprecated_by, component)
         end

         if component_type == 0 then
            if ctx.cfg.auto_related_features then
               ctx.features[component_id] = auto_
            end
            for i = 1, #func.refs do
               local ref = func.refs[i]
               if check_feature(ctx, ref.check, component, version_major, version_minor) then
                  ref.ok = op == 1
               end
            end
         else
            if ctx.cfg.auto_related_extensions then
               ctx.extensions[component_id] = auto_
            end
            for i = 1, #func.refs do
               local ref = func.refs[i]
               if check_extension(ctx, ref.check, component) then
                  ref.ok = op == 1
               end
            end
         end
      end)

   local badrefs
   local deprecatedrefs
   for i = 1, #func.refs do
      local ref = func.refs[i]
      if ref.ok == nil then
         badrefs = tappend(badrefs, ref)
      elseif ref.ok == false then
         deprecatedrefs = tappend(deprecatedrefs, ref)
      elseif ref.ok == true then
         ctx.commands[id] = true
      end
   end

   if deprecatedrefs then
      be.log.warning('Command ' .. func.name .. ' is deprecated!',
         log_refs(deprecatedrefs, log_attr('Deprecated By', deprecated_by, log_attr('Defined By', defined_by))))
   end

   if badrefs then
      be.log.warning('Command ' .. func.name .. ' is not defined by any allowed features or extensions!',
         log_refs(badrefs, log_attr('Deprecated By', deprecated_by, log_attr('Defined By', defined_by))))
   end
end

------------------------------------------------------------------------------
local function handle_command (ctx, func)
   local id = q('SELECT id FROM commands WHERE name = ? LIMIT 1', func.name)

   if not id then return end

   be.log.short_verbose('Processing Command ' .. func.name .. ' (' .. id .. ')')

   local cfg = ctx.cfg.commands[func.name]
   if cfg == nil or cfg == auto_ then
      -- normal
      handle_command_auto(ctx, func, id)

   elseif cfg == false then
      -- disallowed unconditionally (error)
      be.log.warning('Command ' .. func.name .. ' is not allowed!',
         log_refs(func.refs))

   elseif cfg == ignored_ then
      -- ignored (no codegen)

   elseif cfg == true then
      -- allowed unconditionally
      ctx.commands[id] = true

   else
      local t = type(cfg)
      if t == 'string' or t == 'table' then
         -- require specific check string
         local badrefs
         for i = 1, #func.refs do
            local ref = func.refs[i]
            local ok
            if t == 'string' then
               if ref.check == cfg then ok = true end
            else
               for c = 1, #cfg do
                  if ref.check == cfg[c] then ok = true end
               end
            end
            if not ok then badrefs = tappend(badrefs, ref) end
         end
         if badrefs then
            be.log.warning('Command ' .. func.name .. ' is not allowed unchecked!',
               log_refs(badrefs, log_attr('Allowed Checks', cfg)))
         else
            ctx.commands[id] = true
         end
      else
         be.log.warning('Command ' .. func.name .. ' configuration is invalid; ignoring symbol.',
            log_refs(func.refs))
      end
   end

   if ctx.commands[id] then
      ctx.commands[id] = func.weight
   end
   return true
end

------------------------------------------------------------------------------
local function handle_constant (ctx, constant)
   local cfg = ctx.cfg.symbols[constant.name]
   if cfg == false then
      -- disallowed unconditionally (error)
      be.log.warning('Symbol ' .. constant.name .. ' is not allowed!',
         log_refs(constant.refs))
      return
   elseif cfg == ignored_ then
      -- ignored (no codegen)
      return
   end

   if handle_enum(ctx, constant) or
      handle_extension(ctx, constant) or
      handle_feature(ctx, constant) then else
      be.log.notice('Symbol ' .. constant.name .. ' is not a recognized enum, extension, or feature.',
         log_refs(constant.refs))
   end
end

------------------------------------------------------------------------------
local function handle_function (ctx, func)
   local cfg = ctx.cfg.symbols[func.name]
   if cfg == false then
      -- disallowed unconditionally (error)
      be.log.warning('Symbol ' .. func.name .. ' is not allowed!',
         log_refs(func.refs))
      return
   elseif cfg == ignored_ then
      -- ignored (no codegen)
      return
   end

   if handle_command(ctx, func) then else
      be.log.notice('Symbol ' .. func.name .. ' is not a recognized command.',
         log_refs(func.refs))
   end
end

------------------------------------------------------------------------------
local function make_context ()
   local ctx = {
      types = { },
      groups = { },
      enums = { },
      commands = { },
      features = { },
      extensions = { },
      cfg = config,
      api_id = q('SELECT id FROM apis WHERE name = ? LIMIT 1', config.api),
      profile_id = q('SELECT id FROM profiles WHERE name = ? LIMIT 1', config.profile),
      version_major = 0,
      version_minor = 0
   }
   local major, minor = string.match(config.target_version, '(%d+)%.(%d+)')
   ctx.version_major, ctx.version_minor = tonumber(major), tonumber(minor)
   if not ctx.api_id then error('API ' .. config.api .. ' not found!') end
   if not ctx.profile_id then error('Profile ' .. config.profile .. ' not found!') end
   return ctx
end

------------------------------------------------------------------------------
function generate (ctx)
   local sorted_features = make_sorted(ctx.features)
   local sorted_extensions = make_sorted(ctx.extensions)
   local sorted_types = make_sorted(ctx.types)
   local sorted_groups = make_sorted(ctx.groups)
   local sorted_enums = make_sorted(ctx.enums)
   local sorted_commands = make_sorted(ctx.commands, function (a, b)
         if ctx.commands[a.id] == ctx.commands[b.id] then
            return a.id < b.id
         else
            return ctx.commands[a.id] > ctx.commands[b.id]
         end
      end)

   local comment = q 'SELECT comment FROM comments ORDER BY id LIMIT 1'

   -- types --
   for i = 1, #sorted_types do
      local t = sorted_types[i]
      t.name, t.decl, t.comment = q([[
         SELECT name, declaration, comment FROM api_types
         WHERE type_id = ? AND (api_id = ? OR api_id IS NULL)
         ORDER BY api_id DESC LIMIT 1]], t.id, ctx.api_id)

      t.decl = t.decl:gsub('\xEF\xBF\xBD', 'GLAPIENTRY')
      t.full_decl = t.decl

      if t.comment then
         t.extra = ' // ' .. t.comment
         t.full_decl = t.full_decl .. t.extra
      end

      if t.name == 'stddef' or t.name == 'inttypes' then
         t.hide = true
      end
   end


   -- enums --
   local max_enum_length = 0
   local enums = { }
   local enum_reverse_lookup = { }
   for i = 1, #sorted_enums do
      local e = sorted_enums[i]
      e.name, e.preferred_name, e.value, e.raw_value, e.suffix, e.comment = q([[
         SELECT name, preferred_name, value, raw_value, suffix, comment
         FROM api_enums
         WHERE enum_id = ?
         AND (api_id = ? OR api_id IS NULL)
         ORDER BY api_id DESC LIMIT 1
         ]], e.id, ctx.api_id)

      if e.suffix == nil and enum_reverse_lookup[e.value] == nil then
         enum_reverse_lookup[e.value] = e
      end

      e.transformed_name = e.name:gsub('^GL_%f[A-Z]',''):lower()
      if reserved_identifiers[e.transformed_name] then
         e.transformed_name = e.name:lower()
      end

      if e.preferred_name then
         e.extra = { ' // Aliases ', e.preferred_name }
      else
         e.extra = { }
      end

      local prev_op
      q([[
         SELECT component, op, profile
         FROM enum_components
         WHERE enum_id = ?
         AND (api_id = ? OR api_id IS NULL)
         AND (profile_id = ? OR profile_id IS NULL)
         ORDER BY op DESC
         ]], { e.id, ctx.api_id, ctx.profile_id },
         function (component, op, profile)
            if op ~= prev_op then
               prev_op = op
               if op == 1 then
                  e.extra[#e.extra + 1] = ' // '
               else
                  e.extra[#e.extra + 1] = ' // Deprecated: '
               end
            else
               e.extra[#e.extra + 1] = ', '
            end
            e.extra[#e.extra + 1] = component:gsub('GL_', ''):gsub('VERSION_(%d+)_(%d+)', 'GL %1.%2')
            if profile then
               e.extra[#e.extra + 1] = ' ('
               e.extra[#e.extra + 1] = profile
               e.extra[#e.extra + 1] = ')'
            end
         end)

      if e.comment then
         e.extra[#e.extra + 1] = ' // '
         e.extra[#e.extra + 1] = e.comment
      end

      e.extra = table.concat(e.extra)

      enums[e.id] = e
      max_enum_length = math.max(max_enum_length, #e.name)
   end
   local enum_values = { }
   for k,v in pairs(enum_reverse_lookup) do
      enum_values[#enum_values + 1] = v
   end
   table.sort(enum_values, function (a, b) return a.value < b.value end)


   -- groups --
   local empty_groups = { }
   for i = 1, #sorted_groups do
      local g = sorted_groups[i]
      local enums_used_set = { } -- prevent duplicate symbols if api specific enum overrides null declaration
      g.name = q('SELECT name FROM groups WHERE id = ? LIMIT 1', g.id)
      g.enums = { }
      g.max_name_length = 0

      q('SELECT enum_id FROM group_enums WHERE group_id = ? ORDER BY enum_id ASC', { g.id },
         function (enum_id)
            local e = enums[enum_id]
            if e and not enums_used_set[enum_id] then
               g.enums[#g.enums + 1] = e
               g.max_name_length = math.max(g.max_name_length, #e.name)
               enums_used_set[enum_id] = true
            end
         end)

      if #g.enums == 0 then
         g.empty = true
         empty_groups[g.id] = true
      end
   end


   -- commands --
   local max_command_length = 0
   local commands = { }
   local previous_command
   for i = 1, #sorted_commands do
      local c = sorted_commands[i]
      c.prev_command = previous_command
      c.params = { }
      q([[
         SELECT param_index, declaration, name
         FROM command_signature
         WHERE command_id = ?
         ORDER BY param_index ASC
         ]], { c.id },
         function (param_index, declaration, name)
            if param_index == 0 then
               c.name = name
               c.return_decl = declaration
               c.fp_name = name:upper() .. '_PROC'
               max_command_length = math.max(max_command_length, #name)
            else
               local p = {
                  decl = declaration,
                  name = name
               }
               c.params[#c.params + 1] = p
            end
         end)

      local command_aliases = { }
      q([[
         SELECT preferred_id, preferred_name, alias_id, alias_name, type
         FROM command_alias_names WHERE preferred_id = ? OR alias_id = ?
         ]], { c.id, c.id },
         function (pref_id, pref_name, alias_id, alias_name, type)
            if type == 0 then
               if pref_id ~= c.id then
                  command_aliases[pref_id] = pref_name
               elseif alias_id ~= c.id then
                  command_aliases[alias_id] = alias_name
               end
            elseif alias_id == c.id then
               c.vecequiv = pref_name
            end
         end)

      c.aliases = { }
      for k in pairs(command_aliases) do
         c.aliases[#c.aliases + 1] = k
      end
      table.sort(c.aliases)
      for j = 1, #c.aliases do c.aliases[j] = command_aliases[c.aliases[j]] end

      c.defined_by = { }
      c.deprecated_by = { }
      q([[
         SELECT component, op, profile
         FROM command_components
         WHERE command_id = ?
         AND (api_id = ? OR api_id IS NULL)
         AND (profile_id = ? OR profile_id IS NULL)
         ORDER BY op DESC
         ]], { c.id, ctx.api_id, ctx.profile_id },
         function (component, op, profile)
            local map
            if op == 1 then
               map = c.defined_by
            else
               map = c.deprecated_by
            end
            local name = component:gsub('GL_', ''):gsub('VERSION_(%d+)_(%d+)', 'GL %1.%2')
            if name == 'GL 1.0' or name == 'GL 1.1' then
               c.win32native = true
            end
            if profile then
               name = name .. ' (' .. profile .. ')'
            end
            map[#map + 1] = name
         end)

      c.comment = q([[SELECT comment FROM commands WHERE id = ?]], c.id)

      previous_command = c
      commands[c.id] = c
   end
   previous_command = nil
   for i = #sorted_commands, 1, -1 do
      local f = commands[sorted_commands[i].id]
      f.next_command = previous_command
      previous_command = f
   end


   -- features --
   local features = { }
   local previous_feature
   for i = 1, #sorted_features do
      local f = sorted_features[i]
      f.name, f.version_major, f.version_minor, f.comment = q([[
         SELECT name, version_major, version_minor, comment
         FROM components WHERE id = ? LIMIT 1
         ]], f.id)

      f.prev_feature = previous_feature
      f.ctx_name = f.name:lower()
      f.func_name = f.name:gsub('GL_', ''):gsub('VERSION_(%d+)_(%d+)', 'GL%1%2'):lower()

      f.commands = { }
      q([[
         SELECT command_id FROM component_commands
         WHERE component_id = ?
         AND (api_id = ? OR api_id IS NULL)
         AND (profile_id = ? OR profile_id IS NULL)
         AND op = 1 ORDER BY command_id ASC
         ]], { f.id, ctx.api_id, ctx.profile_id },
         function (command_id)
            if commands[command_id] then
               f.commands[#f.commands + 1] = commands[command_id]
            end
         end)

      previous_feature = f
      features[f.id] = f
   end
   previous_feature = nil
   for i = #sorted_features, 1, -1 do
      local f = features[sorted_features[i].id]
      f.next_feature = previous_feature
      previous_feature = f
   end


   -- extensions --
   local extension_hashes = { }
   for i = 1, #sorted_extensions do
      local x = sorted_extensions[i]
      x.name, x.comment = q([[
         SELECT name, comment
         FROM components WHERE id = ? LIMIT 1
         ]], x.id)

      x.ctx_name = x.name:lower()
      x.func_name = x.name:gsub('GL_', ''):lower()
      x.hash = be.Id(x.name)
      if extension_hashes[x.hash] then
         be.log.error('Extension name hash collision!', { 'Extension 1', 'Extension 2' }, { extension_hashes[x.hash].name, x.name })
      else
         extension_hashes[x.hash] = x
      end

      x.commands = { }
      q([[
         SELECT command_id FROM component_commands
         WHERE component_id = ?
         AND (api_id = ? OR api_id IS NULL)
         AND (profile_id = ? OR profile_id IS NULL)
         AND op = 1 ORDER BY command_id ASC
         ]], { x.id, ctx.api_id, ctx.profile_id },
         function (command_id)
            if commands[command_id] then
               x.commands[#x.commands + 1] = commands[command_id]
            end
         end)
   end

   write_template(ctx.cfg.template, {
      comment = comment,
      features = sorted_features,
      extensions = sorted_extensions,
      types = sorted_types,
      groups = sorted_groups,
      enums = sorted_enums,
      enum_values = enum_values,
      commands = sorted_commands,

      skip_groups = ctx.cfg.skip_groups,
      max_enum_name_length = max_enum_length,
      max_command_name_length = max_command_length,
   })
end

------------------------------------------------------------------------------
function process_results (ctx)
   if ctx.cfg.skip_codegen then
      local function concat_keys (t, sep)
         local l, n = { }, 0
         for k in pairs(t) do
            n = n + 1
            l[n] = tostring(k)
         end
         return table.concat(l, sep)
      end

      local attrs = { 'Features', 'Extensions', 'Commands', 'Enums', 'Groups', 'Types' }
      local vals = { concat_keys(ctx.features, ', '),
         concat_keys(ctx.extensions, ', '),
         concat_keys(ctx.commands, ', '),
         concat_keys(ctx.enums, ', '),
         concat_keys(ctx.groups, ', '),
         concat_keys(ctx.types, ', '),
      }

      be.log.info('Context', attrs, vals)
   else
      generate (ctx)
   end
end

------------------------------------------------------------------------------
function process ()
   local ctx = make_context()

   for i = 1, #found_constants do
      handle_constant(ctx, found_constants[i])
   end
   for i = 1, #found_functions do
      handle_function(ctx, found_functions[i])
   end

   -- make sure all features <= target feature are added to id set
   q('SELECT component_id FROM features WHERE api_id = ? AND (version_major * 1000 + version_minor) <= ?',
      { ctx.api_id, ctx.version_major * 1000 + ctx.version_minor },
      function (feature_id)
         ctx.features[feature_id] = true
      end)

   -- make sure that all things that were explicitly enabled in the config are in the id sets
   local cfg = ctx.cfg
   for feature, v in pairs(cfg.features) do
      if v == true or v == auto_ then
         local id = q('SELECT component_id FROM features WHERE name = ? AND api_id = ? LIMIT 1', feature, ctx.api_id)
         if id then ctx.features[id] = true end
      end
   end
   for extension, v in pairs(cfg.extensions) do
      if v == true or v == auto_ then
         local id = q('SELECT component_id FROM extensions WHERE name = ? AND api_id = ? LIMIT 1', extension, ctx.api_id)
         if id then ctx.extensions[id] = true end
      end
   end
   for command, v in pairs(cfg.commands) do
      if v == true or v == auto_ then
         q([[
            SELECT command_id, component_id, component_type
            FROM command_components WHERE name = ?
            AND (api_id = ? OR api_id IS NULL)
            AND (profile_id = ? OR profile_id IS NULL)
            ]], { command, ctx.api_id, ctx.profile_id },
            function (command_id, component_id, component_type)
               ctx.commands[command_id] = true
               if component_type == 0 then
                  ctx.features[component_id] = true
               else
                  ctx.extensions[component_id] = true
               end
            end)
      end
   end
   for enum, v in pairs(cfg.enums) do
      if v == true or v == auto_ then
         q([[
            SELECT enum_id, component_id, component_type
            FROM enum_components WHERE name = ?
            AND (api_id = ? OR api_id IS NULL)
            AND (profile_id = ? OR profile_id IS NULL)
            ]], { enum, ctx.api_id, ctx.profile_id },
            function (enum_id, component_id, component_type)
               ctx.enums[enum_id] = true
               if component_type == 0 then
                  ctx.features[component_id] = true
               else
                  ctx.extensions[component_id] = true
               end
            end)
      end
   end

   -- add types and groups used by commands
   for id in pairs(ctx.commands) do
      q('SELECT type_id, group_id FROM command_signature WHERE command_id = ?', { id },
         function (type_id, group_id)
            if type_id then ctx.types[type_id] = true end
            if group_id then ctx.groups[group_id] = true end
         end)
   end

   -- add enums and types defined by features
   local sorted_features = { }
   local n = 0
   for id in pairs(ctx.features) do
      n = n + 1
	  sorted_features[n] = id
   end
   table.sort(sorted_features)
   for i = 1, #sorted_features do
	  local id = sorted_features[i]
      q([[
         SELECT enum_id, op FROM component_enums
         WHERE component_id = ?
         AND (api_id = ? OR api_id IS NULL)
         AND (profile_id = ? OR profile_id IS NULL)
         ]], { id, ctx.api_id, ctx.profile_id },
         function (enum_id, op)
		    if ctx.enums[enum_id] == nil or ctx.enums[enum_id] == auto_ then
			   if op == 1 then
				  ctx.enums[enum_id] = auto_
			   else
				  ctx.enums[enum_id] = nil
			   end
			end
         end)
      q([[
         SELECT type_id FROM component_types
         WHERE component_id = ?
         AND (api_id = ? OR api_id IS NULL)
         AND (profile_id = ? OR profile_id IS NULL)
         ]], { id, ctx.api_id, ctx.profile_id },
         function (type_id)
            ctx.types[type_id] = true
         end)
   end

   -- add enums and types defined by extensions
   for id in pairs(ctx.extensions) do
      q([[
         SELECT enum_id FROM component_enums
         WHERE component_id = ?
         AND (api_id = ? OR api_id IS NULL)
         AND (profile_id = ? OR profile_id IS NULL)
         ]], { id, ctx.api_id, ctx.profile_id },
         function (enum_id)
            ctx.enums[enum_id] = ctx.enums[enum_id] or auto_
         end)
      q([[
         SELECT type_id FROM component_types
         WHERE component_id = ?
         AND (api_id = ? OR api_id IS NULL)
         AND (profile_id = ? OR profile_id IS NULL)
         ]], { id, ctx.api_id, ctx.profile_id },
         function (type_id)
            ctx.types[type_id] = true
         end)
   end

   -- always add GLenum, etc type just in case
   q([[SELECT id FROM types WHERE name IN ('GLenum', 'GLuint', 'GLsizei', 'GLchar')]], { },
      function (id)
         ctx.types[id] = true
      end)

   -- enums needed for codegen
   q([[SELECT id FROM enums WHERE name IN ('GL_VERSION', 'GL_EXTENSIONS', 'GL_NUM_EXTENSIONS')]], { },
      function (id)
         ctx.enums[id] = true
      end)

   -- add any dependencies of other types
   local dep_types
   for id in pairs(ctx.types) do
      q([[
         SELECT nondependent_type_id FROM type_dependencies
         WHERE dependent_type_id = ?
         AND (api_id = ? OR api_id IS NULL)
         ]], { id, ctx.api_id },
      function (id)
         dep_types = tappend(dep_types, id)
      end)
   end
   if dep_types then
      for i = 1, #dep_types do
         ctx.types[dep_types[i]] = true
      end
   end

   process_results(ctx)
end

function import_bglgenrc (path)
   local p = fs.compose_path(path, '.bglgenrc')
   if fs.exists(p) then
      bglgenrc_path = p
      root_dir = path
      dofile(p)
      return true
   end

   if fs.root_path(path) == path then
      root_dir = path
      return false
   end

   return import_bglgenrc(fs.parent_path(path))
end

import_bglgenrc(fs.cwd())
