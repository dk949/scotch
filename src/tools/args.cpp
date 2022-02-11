#include "args.hpp"

#include "ftrace.hpp"

#include <vector>  // for vector

std::vector<StringView> Tools::Args::positionals = {};

void Tools::Args::parse(char **argv) {
    ftrace();
    for (const char *arg = *(++argv); arg; arg = *(++argv)) {
        positionals.push_back(arg);
    }
}
