#include "ftrace.hpp"

#include "log.hpp"


char Tools::Ftrace::indent[128] = {0};
int Tools::Ftrace::level = 0;


Tools::Ftrace::Ftrace(std::string_view func)
        : m_func(func)
        , m_sw() {
    trace_init();
    ftraceLogger->trace("{}Enter {}", indent, m_func);
    indent[level++] = ' ';
    indent[level++] = ' ';
}

Tools::Ftrace::~Ftrace() {
    indent[--level] = '\0';
    indent[--level] = '\0';
    ftraceLogger->trace("{}Exit {}. Took {}s", indent, m_func, m_sw);
}
