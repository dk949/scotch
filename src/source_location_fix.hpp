#ifndef SOURCE_LOCATION_FIX_HPP
#define SOURCE_LOCATION_FIX_HPP

// Clang doesn't have source_location
#ifndef __clang__
#include <source_location>
using sloc = std::source_location;
#else
// fake source location for clang
// clang-format off
struct sloc {
    static sloc current() {return sloc {};}
    const char *file_name() const {return "0";}
    const char *function_name() const {return file_name();}
    const char *column() const {return file_name();}
    const char *line() const {return file_name();}
};
// clang-format on
#endif

#endif  // SOURCE_LOCATION_FIX_HPP
