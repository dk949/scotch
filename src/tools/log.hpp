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
#define trace_init()
#endif  // TRACE


#define log_init()                                         \
    namespace Tools {                                      \
        [[maybe_unused]] static const auto setLog = []() { \
            spdlog::set_pattern("%^[%l]%$: %v");           \
            SET_LOG_LEVEL();                               \
            return 0;                                      \
        }();                                               \
    }

// clang-format off
#define todo()                                         \
    do {                                               \
        const auto loc = sloc::current();              \
        spdlog::critical("{}:{}:{}: `{}` not yet implemented", \
            Tools::filename(loc.file_name()),          \
            loc.line(),                                \
            loc.column(),                              \
            loc.function_name());                      \
        exit(1);                                       \
    } while (0)





#define unreachable(...)                        \
    do {                                        \
        const auto loc = sloc::current();       \
        spdlog::critical("{}:{}:{}: Unreachable: {}",   \
            Tools::filename(loc.file_name()),   \
            loc.line(),                         \
            loc.column(),                       \
            fmt::format(__VA_ARGS__));          \
        exit(1);                                \
    } while (0)

#define fixme(...)                                                                         \
    do {                                                                                   \
        const auto loc = sloc::current();                                                  \
        const auto file = Tools::filename(loc.file_name());                                \
        const auto line = loc.line();                                                      \
        const auto col = loc.column();                                                     \
        if(!WarnCache::isCached(Tools::fnv_1a(file) ^ line ^ ~static_cast<uint64_t>(col))) \
            spdlog::warn("{}:{}:{}: FIXME: {}", file, line, col, fmt::format(__VA_ARGS__));        \
    } while (0)

#define crash(...)                              \
    do {                                        \
        const auto loc = sloc::current();       \
        spdlog::critical("{}:{}:{}: Fatal error: {}",   \
            Tools::filename(loc.file_name()),   \
            loc.line(),                         \
            loc.column(),                       \
            fmt::format(__VA_ARGS__));          \
        exit(1);                                \
    } while (0)


#define verify(X)                                               \
    do {                                                        \
        if((X)) break;                                          \
        const auto loc = sloc::current();                       \
        spdlog::critical("{}:{}:{}: Assertion failed: " #X " is false", \
            Tools::filename(loc.file_name()),                   \
            loc.line(),                                         \
            loc.column());                                      \
        exit(1);                                                \
    } while (0)

#define verify_msg(X, ...)                                               \
    do {                                                                 \
        if((X)) break;                                                   \
        const auto loc = sloc::current();                                \
        spdlog::critical("{}:{}:{}: Assertion failed: (" #X ") is false\n\t {}", \
            Tools::filename(loc.file_name()),                            \
            loc.line(),                                                  \
            loc.column(),                                                \
            fmt::format(__VA_ARGS__));                                   \
        exit(1);                                                         \
    } while (0)

// clang-format on
#endif  // LOGGING_HPP
