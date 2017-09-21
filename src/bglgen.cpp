#include "bglgen_app.hpp"

#ifdef BE_DEBUG
#pragma comment(lib, "sqlite-debug")
#else
#pragma comment(lib, "sqlite")
#endif

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
   be::bglgen::BglGenApp app(argc, argv);
   return app();
}
