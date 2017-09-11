#pragma once
#ifndef BE_BGLGEN_BGLGEN_APP_HPP_
#define BE_BGLGEN_BGLGEN_APP_HPP_

#include <be/core/lifecycle.hpp>
#include <be/core/be.hpp>

namespace be::bglgen {

///////////////////////////////////////////////////////////////////////////////
class BglGenApp final {
public:
   BglGenApp(int argc, char** argv);

   int operator()();

private:
   CoreInitLifecycle init_;
   be::I8 status_ = 0;
};

} // be::bglgen

#endif
