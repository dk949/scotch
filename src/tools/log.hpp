#ifndef LOGGING_HPP
#define LOGGING_HPP
#include "hash.hpp"
#include "source_location_fix.hpp"
#include "warncahce.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <string_view>

namespace Tools {
constexpr static std::string_view filename(std::string_view sv) {
    for (auto it = sv.rbegin(); it != sv.rend(); it++) {
#ifdef _WIN32
        if (*it == '\\')
#else
        if (*it == '/')
#endif
            return std::string_view {it.base(), sv.end()};
    }
    return sv;
}
}

static std::shared_ptr<spdlog::logger> ftraceLogger;

#ifdef TRACE
#define SET_LOG_LEVEL() spdlog::set_level(spdlog::level::trace)
#define trace_init()                                                             \
    if (ftraceLogger == nullptr) {                                               \
        ftraceLogger = spdlog::basic_logger_mt("trace", "logs/trace.log", true); \
        ftraceLogger->set_pattern("[%T.%f] %v");                                 \
    }

#else
#define SET_LOG_LEVEL() spdlog::set_level(spdlog::level::debug)
#define trcae_init()
#endif  // TRACE


#define log_init()                                         \
    namespace Tools {                                      \
        [[maybe_unused]] static const auto setLog = []() { \
            spdlog::set_pattern("%^[%l]%$: %v");           \
            SET_LOG_LEVEL();                               \
            return 0;                                      \
        }();                                               \
    }

#define debug(...)    spdlog::debug(__VA_ARGS__)
#define info(...)     spdlog::info(__VA_ARGS__)
#define warn(...)     spdlog::warn(__VA_ARGS__)
#define error(...)    spdlog::error(__VA_ARGS__)
#define critical(...) spdlog::critical(__VA_ARGS__)

// clang-format off
#define todo()                                         \
    do {                                               \
        const auto loc = sloc::current();              \
        critical("{}:{}:{}: `{}` not yet implemented", \
            Tools::filename(loc.file_name()),          \
            loc.line(),                                \
            loc.column(),                              \
            loc.function_name());                      \
        exit(1);                                       \
    } while (0)





#define unreachable(MSG, ...)                   \
    do {                                        \
        const auto loc = sloc::current();       \
        critical("{}:{}:{}: Unreachable: " MSG, \
            Tools::filename(loc.file_name()),   \
            loc.line(),                         \
            loc.column(),                       \
            __VA_ARGS__);                       \
        exit(1);                                \
    } while (0)

#define fixme(MSG, ...)                                                 \
    do {                                                                \
        const auto loc = sloc::current();                               \
        const auto file = Tools::filename(loc.file_name());             \
        const auto line = loc.line();                                   \
        const auto col = loc.column();                                  \
        if(!WarnCache::isCached(Tools::fnv_1a(file) ^ line ^ ~col))     \
            warn("{}:{}:{}: FIXME: " MSG, file, line, col, __VA_ARGS__);\
    } while (0)

#define crash(MSG, ...)                         \
    do {                                        \
        const auto loc = sloc::current();       \
        critical("{}:{}:{}: Fatal error: " MSG, \
            Tools::filename(loc.file_name()),   \
            loc.line(),                         \
            loc.column(),                       \
            __VA_ARGS__);                       \
        exit(1);                                \
    } while (0)


#define verify(X)                                               \
    do {                                                        \
        if((X)) break;                                          \
        const auto loc = sloc::current();                       \
        critical("{}:{}:{}: Assertion failed: " #X " is false", \
            Tools::filename(loc.file_name()),                   \
            loc.line(),                                         \
            loc.column());                                      \
        exit(1);                                                \
    } while (0)

#define verify_msg(X, MSG, ...)                                          \
    do {                                                                 \
        if((X)) break;                                                   \
        const auto loc = sloc::current();                                \
        critical("{}:{}:{}: Assertion failed: (" #X ") is false\n\t" MSG,\
            Tools::filename(loc.file_name()),                            \
            loc.line(),                                                  \
            loc.column(),                                                \
            __VA_ARGS__);                                                \
        exit(1);                                                         \
    } while (0)

// clang-format on
#endif  // LOGGING_HPP
