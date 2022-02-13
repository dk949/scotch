#ifndef FTRACE_HPP
#define FTRACE_HPP

#include "class_tools.hpp"
#include "source_location_fix.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include <string_view>

namespace Tools {
struct Ftrace {
public:
    NO_MOVE_OR_COPY(Ftrace);

private:
    std::string_view m_func;
    spdlog::stopwatch m_sw;

    static constexpr auto maxIndent = 128;
    static char indent[maxIndent];
    static int level;

public:
    explicit Ftrace(std::string_view func);
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
