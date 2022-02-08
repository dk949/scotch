#include "args.hpp"

#include <types.hpp>  // for StringView
#include <vector>     // for vector

std::vector<StringView> Args::positionals = {};

void Args::parse(char **argv) {
    for (const char *arg = *(++argv); arg; arg = *(++argv)) {
        positionals.push_back(arg);
    }
}
