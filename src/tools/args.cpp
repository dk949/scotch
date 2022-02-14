#include "args.hpp"

#include "ftrace.hpp"
#include "log.hpp"
#include "switch_tools.hpp"

#include <vector>  // for vector

std::vector<StringView> Tools::Args::positionals = {};
StringView Tools::Args::output = "out.wasm";

void Tools::Args::parse(char **argv) {
    ftrace();
    auto *prog = *argv;
    for (const char *arg = *(++argv); arg; arg = *(++argv)) {
        // clang-format off
        str_switch(arg) {
            str_case("-h"):
            str_case("--help"):
                print("Usage: {} IN_FILE [-o OUT_FILE]\n", Tools::filename(prog));
                exit(0);
            str_case("-o"):
            str_case("--output"):
                output = *(++argv);
        }
        // clang-format on
        positionals.emplace_back(arg);
    }

    if (positionals.empty()) {
        crash("Expected file, got nothing");
    }
}
