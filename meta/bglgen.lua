-- Predefined global variables:
-- found_functions = { { name = 'glXXXXX', weight = 100, used_unchecked = true, refs = { { path = '...', weight = 100, checked = true } }, { ... }, ... }
-- found_constants = { { name = 'GL_XXXXX', weight = 100, used_unchecked = true, refs = { { path = '...', weight = 100, checked = true } }, { ... }, ... }

-- Predefined gl_registry functions:
-- list_features
-- list_extensions
-- get_feature
-- get_extension
-- get_command
-- get_enum
-- get_type
-- get_group

local table = table
local debug = debug
local string = string
local type = type
local select = select
local ipairs = ipairs
local dofile = dofile
local getmetatable = getmetatable
local setmetatable = setmetatable

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
   local n = 1

   function section (s)
      if not sections[s] then
	     sections[s] = { }
	  end

	  out = sections[s]
	  n = #out + 1
   end

   local function init ()
      reset_indent()
      section(0)
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
	  local strtable = { }
	  for i = 1, #sections do
		 strtable[#strtable + 1] = table.concat(sections[i])
	  end

      local str = table.concat(strtable)
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

local ignored_ = { }
ignored = ignored_

config = {
   api = 'gl',
   target_version = '3.3',
   profile = 'core',
   features = { },
   extensions = { },
   enums = { },
   commands = { },
   symbols = { GL_ALWAYS = false }
}

function process ()
   local enums = { }
   local groups = { }
   local types = { }
   local commands = { }
   local extensions = { }

   for i = 1, #found_constants do
      local constant = found_constants[i]
      local symbol_config = config.symbols[constant.name]

      if symbol_config == false then
         -- disallowed unconditionally (error)
         local attrs = { }
         local vals = { }
         for r = 1, #constant.refs do
            local ref = constant.refs[r]
            attrs[#attrs+1] = 'Reference'
            vals[#vals+1] = ref.path .. ':' .. ref.line
         end
         be.log.warning(constant.name .. ' is not allowed!', attrs, vals)
         goto continue
      elseif symbol_config == ignored_ then
         -- ignored (no codegen)
         goto continue
      end

      local feature, extension, enum

      feature = gl_registry.get_feature(constant.name, config.api)
      if feature then



         goto continue
      end

      extension = gl_registry.get_extension(constant.name, config.api)
      if extension then

         goto continue
      end

      enum = gl_registry.get_enum(constant.name, config.api)
      if enum then

         goto continue
      end

      be.log.warning(constant.name .. ' not recognized!')




      ::continue::
   end

   -- for i = 1, #found_functions do
   --    local fn = found_functions[i]
   --    local cmd = gl_registry.get_command(fn.name)
   -- end
end


-- util.print_r(found_constants)
-- util.print_r(found_functions)



-- util.print_r(gl_registry.list_features("gl"))
-- util.print_r(gl_registry.list_extensions("gl"))
-- util.print_r(gl_registry.get_feature("GL_VERSION_1_0", "gles"))
-- util.print_r(gl_registry.get_extension("GL_KHR_debug", "gl"))
--util.print_r(gl_registry.get_command("glVertexArrayVertexAttribOffsetEXT"))


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
