#ifndef ARGS_HPP
#define ARGS_HPP
#include "tools.hpp"
#include "types.hpp"


struct Args {
    static Vector<StringView> positionals;
    static void parse(char **argv);
};


#endif  // ARGS_HPP
