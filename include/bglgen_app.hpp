#pragma once
#ifndef BE_BGLGEN_BGLGEN_APP_HPP_
#define BE_BGLGEN_BGLGEN_APP_HPP_

#include <be/core/lifecycle.hpp>
#include <be/core/filesystem.hpp>

namespace be::bglgen {

///////////////////////////////////////////////////////////////////////////////
class BglGenApp final {
public:
   BglGenApp(int argc, char** argv);

   int operator()();

private:
   CoreInitLifecycle init_;
   be::I8 status_ = 0;

   Path registry_location_;
   Path registry_db_location_;
   bool rebuild_db_;

   std::vector<Path> source_paths_;
   std::vector<S> lua_chunks_;
};

} // be::bglgen

#endif
