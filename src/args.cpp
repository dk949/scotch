#include "args.hpp"

std::vector<StringView> Args::positionals = {};

void Args::parse(char **argv) {
    for (const char *arg = *(++argv); arg; arg = *(++argv)) {
        positionals.push_back(arg);
    }
}
