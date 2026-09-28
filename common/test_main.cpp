/*
  The main routine for testing and benchmarking. Add options to the standard Catch2 options.
  See ../README.md for details.
  
  Based on code in https://github.com/catchorg/Catch2/blob/v2.13.10/docs/own-main.md#adding-your-own-command-line-options
  See that page for copyright information.
 */

#include <format>

#include "benchmark_utils.hpp"

// Import definitions necessary to add CLI options
#define CATCH_CONFIG_RUNNER
// Define global benchmarking variables
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch2.hpp"

int main( int argc, char* argv[] ) {
  Catch::Session session;

  // Build a new parser on top of Catch2's
  using namespace Catch::clara;
  auto cli
    = session.cli()
    | Opt( benchmark_utils::traversals, "traversals" )
        ["--traversals"]
        (std::format("Number of traversals to perform (default {})", benchmark_utils::traversals));

  // Now pass the new composite back to Catch2 so it uses that
  session.cli( cli );

  // Let Catch2 (using Clara) parse the command line
  int returnCode = session.applyCommandLine( argc, argv );
  if( returnCode != 0 ) // Indicates a command line error
      return returnCode;

  return session.run();
}
