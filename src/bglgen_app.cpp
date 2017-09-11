#include "bglgen_app.hpp"
#include "version.hpp"
#include <be/core/version.hpp>
#include <be/cli/cli.hpp>
#include <be/core/logging.hpp>
#include <be/core/log_exception.hpp>
#include <be/util/get_file_contents.hpp>
#include <be/util/path_glob.hpp>
#include <be/core/alg.hpp>
#include <iostream>
#include <fstream>

namespace be::bglgen {

///////////////////////////////////////////////////////////////////////////////
BglGenApp::BglGenApp(int argc, char** argv) {
   default_log().verbosity_mask(v::info_or_worse);
   try {
      using namespace cli;
      using namespace color;
      using namespace ct;
      Processor proc;

      bool show_version = false;
      bool show_help = false;
      bool verbose = false;
      S help_query;

      proc
         (prologue (Table() << header << "BENGINE GL EXTENSION LOADER GENERATOR").query())

         (synopsis (Cell() << fg_dark_gray << "[ " << fg_cyan << "OPTIONS"
                           << fg_dark_gray << " ]"))

         (abstract ("TODO"))

         (abstract ("TODO").verbose())

         (end_of_options ())

         (verbosity_param ({ "v" },{ "verbosity" }, "LEVEL", default_log().verbosity_mask()))

         (flag ({ "V" },{ "version" }, show_version).desc("Prints version information to standard output."))

         (param ({ "?" },{ "help" }, "OPTION", [&](const S& value) {
               show_help = true;
               help_query = value;
            }).default_value(S())
              .allow_options_as_values(true)
              .desc(Cell() << "Outputs this help message.  For more verbose help, use " << fg_yellow << "--help")
              .extra(Cell() << nl << "If " << fg_cyan << "OPTION" << reset
                            << " is provided, the options list will be filtered to show only options that contain that string."))

         (flag ({ },{ "help" }, verbose).ignore_values(true))

         ;

      proc.process(argc, argv);

      if (!show_help && !show_version) {
         show_help = true;
         show_version = true;
         status_ = 1;
      }

      if (show_version) {
         proc
            (prologue (BE_BGLGEN_VERSION_STRING).query())
            (prologue (BE_CORE_VERSION_STRING).query())
            (license (BE_LICENSE).query())
            (license (BE_COPYRIGHT).query())
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

   return status_;
}

} // be::bglgen
