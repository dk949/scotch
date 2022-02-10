#ifndef TRACE_HPP
#define TRACE_HPP

#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include <string_view>

namespace tools {
struct Trace {
private:
    std::string_view m_func;
    spdlog::stopwatch m_sw;

    static char indent[128];
    static int level;

public:
    Trace(std::string_view func);
    ~Trace();
};
}

#endif  // TRACE_HPP
