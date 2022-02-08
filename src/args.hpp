#ifndef ARGS_HPP
#define ARGS_HPP
#include "types.hpp"  // for StringView, Vector


struct Args {
    static Vector<StringView> positionals;
    static void parse(char **argv);
};


#endif  // ARGS_HPP
