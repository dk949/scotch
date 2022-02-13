#ifndef SOURCE_LOCATION_FIX_HPP
#define SOURCE_LOCATION_FIX_HPP

// Clang doesn't have source_location
#ifndef __clang__
#include <source_location>
using sloc = std::source_location;
#else
#include <cstdint>
// fake source location for clang
// clang-format off
struct sloc {
    static sloc current() {return sloc {};}
    [[nodiscard]] const char *file_name() const {return "file.?";}
    [[nodiscard]] const char *function_name() const {return "funciton.?";}
    [[nodiscard]] unsigned long long column() const {return 0;}
    [[nodiscard]] uint64_t line() const {return 0;}
};
// clang-format on
#endif

#endif  // SOURCE_LOCATION_FIX_HPP
