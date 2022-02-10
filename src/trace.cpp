#include "trace.hpp"

#include "spdlog/sinks/basic_file_sink.h"


char tools::Trace::indent[128] = {0};
int tools::Trace::level = 0;

static std::shared_ptr<spdlog::logger> logger;


tools::Trace::Trace(std::string_view func)
        : m_func(func)
        , m_sw() {
    if (logger == nullptr) {
        logger = spdlog::basic_logger_mt("trace", "logs/trace.log");
    }
    logger->trace("{}Enter {}", indent, m_func);
    indent[level++] = ' ';
    indent[level++] = ' ';
}

tools::Trace::~Trace() {
    indent[--level] = '\0';
    indent[--level] = '\0';
    logger->trace("{}Exit {}. Took {}s", indent, m_func, m_sw);
}
