#ifndef TOOLS_HPP
#define TOOLS_HPP

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

#include "hash.hpp"
#include "trace.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include <string_view>

namespace tools {
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

template<typename>
constexpr bool always_false_v = false;
}  // namespace tools

#ifdef TRACE
#define SET_LOG_LEVEL() spdlog::set_level(spdlog::level::trace)
#else
#define SET_LOG_LEVEL() spdlog::set_level(spdlog::level::debug)
#endif  // TRACE

#define log_init()                                         \
    namespace tools {                                      \
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
            tools::filename(loc.file_name()),          \
            loc.line(),                                \
            loc.column(),                              \
            loc.function_name());                      \
        exit(1);                                       \
    } while (0)


#ifdef TRACE
#define trace()                                 \
        const auto trace_loc = sloc::current(); \
        tools::Trace trace_trace{trace_loc.function_name()}
#else
#define trace()
#endif // TRACE



#define unreachable(MSG, ...)                   \
    do {                                        \
        const auto loc = sloc::current();       \
        critical("{}:{}:{}: Unreachable: " MSG, \
            tools::filename(loc.file_name()),   \
            loc.line(),                         \
            loc.column(),                       \
            __VA_ARGS__);                       \
        exit(1);                                \
    } while (0)

#define crash(MSG, ...)                         \
    do {                                        \
        const auto loc = sloc::current();       \
        critical("{}:{}:{}: Fatal error: " MSG, \
            tools::filename(loc.file_name()),   \
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
            tools::filename(loc.file_name()),                   \
            loc.line(),                                         \
            loc.column());                                      \
        exit(1);                                                \
    } while (0)

#define verify_msg(X, MSG, ...)                                          \
    do {                                                                 \
        if((X)) break;                                                   \
        const auto loc = sloc::current();                                \
        critical("{}:{}:{}: Assertion failed: (" #X ") is false\n\t" MSG,\
            tools::filename(loc.file_name()),                            \
            loc.line(),                                                  \
            loc.column(),                                                \
            __VA_ARGS__);                                                \
        exit(1);                                                         \
    } while (0)


// clang-format on

#define to static_cast

#define bcase \
    break;    \
    case

#define dev [[maybe_unused]]

#define str_switch(STR) switch (CTHash::fnv_1a((STR)))
#define str_case(STR)   case CTHash::fnv_1a((STR))



#define DEF_MOVE(CLASS)        \
    CLASS(CLASS &&) = default; \
    CLASS &operator=(CLASS &&) = default

#define DEF_COPY(CLASS)             \
    CLASS(const CLASS &) = default; \
    CLASS &operator=(const CLASS &) = default

#define NO_MOVE(CLASS)        \
    CLASS(CLASS &&) = delete; \
    CLASS &operator=(CLASS &&) = delete

#define NO_COPY(CLASS)             \
    CLASS(const CLASS &) = delete; \
    CLASS &operator=(const CLASS &) = delete

#define MOVE_BUT_NO_COPY(CLASS) \
    NO_COPY(CLASS);             \
    DEF_MOVE(CLASS)

#define COPY_BUT_NO_MOVE(CLASS) \
    DEF_COPY(CLASS);            \
    NO_MOVE(CLASS)

#define NO_MOVE_OR_COPY(CLASS) \
    NO_MOVE(CLASS);            \
    NO_COPY(CLASS)

#define DEF_MOVE_AND_COPY(CLASS) \
    DEF_MOVE(CLASS);             \
    DEF_COPY(CLASS)



#define ENUM_DO(E)

#define ENUM_CASE(E) \
    case E:          \
        ENUM_DO(E);  \
        break
#define BAD_ENUM_CASE(E) \
    case E:              \
        unreachable("case {} should never happen", #E);

#endif  // TOOLS_HPP
