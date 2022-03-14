#include "args.hpp"

#include "ftrace.hpp"
#include "log.hpp"
#include "switch_tools.hpp"

#include <vector>


namespace Tools {

Args::Args(Vector<StringView> positionals, StringView output)
        : m_positionals(std::move(positionals))
        , m_output(output) {
    ftrace();
}

Args Args::parse(char **argv) {
    ftrace();
    auto *prog = *argv;
    StringView output;
    Vector<StringView> positionals;
    for (const char *arg = *(++argv); arg; arg = *(++argv)) {
        // clang-format off
        str_switch(arg) {
            str_case("-h"):
            str_case("--help"):
                fmt::print("Usage: {} IN_FILE [-o OUT_FILE]\n", Tools::filename(prog));
                exit(0);
            str_case("-o"):
            str_case("--output"):
                if(*(argv++)) {
                    output = *argv;
                } else {
                    fmt::print("Expected filename after '-o', got nothing");
                    exit(1);
                }
        }
        // clang-format on
        positionals.emplace_back(arg);
    }

    if (positionals.empty()) {
        fmt::print("Expected file, got nothing");
        exit(1);
    }
    if (output.empty()) {
        output = "output.wat";
    }
    return Args(positionals, output);
}


}  // namespace Tools
