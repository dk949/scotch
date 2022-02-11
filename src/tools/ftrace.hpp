#ifndef FTRACE_HPP
#define FTRACE_HPP

#include "source_location_fix.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include <string_view>

namespace Tools {
struct Ftrace {
private:
    std::string_view m_func;
    spdlog::stopwatch m_sw;

    static char indent[128];
    static int level;

public:
    Ftrace(std::string_view func);
    ~Ftrace();
};
}


#ifdef TRACE
#define ftrace()                             \
    const auto ftrace_loc = sloc::current(); \
    Tools::Ftrace ftrace_trace {             \
        ftrace_loc.function_name()           \
    }
#else
#define ftrace()
#endif  // TRACE

#endif  // FTRACE_HPP