#pragma once
#ifndef BE_BGLGEN_BGLGEN_APP_HPP_
#define BE_BGLGEN_BGLGEN_APP_HPP_

#include "lexer.hpp"
#include <be/core/lifecycle.hpp>
#include <be/core/filesystem.hpp>
#include <be/belua/context.hpp>
#include <be/sqlite/db.hpp>
#include <be/sqlite/static_stmt_cache.hpp>

namespace be::bglgen {

///////////////////////////////////////////////////////////////////////////////
class BglGenApp final {
public:
   BglGenApp(int argc, char** argv);

   int operator()();

private:
   void get_lua_defaults_();
   void init_registry_();
   void init_extension_regex_();

   CoreInitLifecycle init_;
   be::I8 status_ = 0;

   belua::Context context_;
   sqlite::Db db_;
   std::unique_ptr<sqlite::StaticStmtCache> cache_;

   Path registry_location_;
   Path registry_db_location_;
   bool rebuild_db_ = false;

   std::vector<S> extensions_;
   S extension_regex_;
   std::vector<std::pair<Path, bool>> search_dirs_;
   std::vector<Path> source_paths_;

   std::vector<S> lua_chunks_;
   bool do_process_ = true;
   
   Path output_location_;
};

} // be::bglgen

#endif
