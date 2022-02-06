#ifndef ARGS_HPP
#define ARGS_HPP
#include "tools.hpp"
#include "types.hpp"


struct args {
    static Vector<StringView> positionals;
    static void parse(char **argv) {
        for (const char *arg = *(++argv); arg; arg = *(++argv)) {
            positionals.push_back(arg);
        }
    }
};

std::vector<StringView> args::positionals = {};

#endif  // ARGS_HPP
