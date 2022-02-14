#ifndef ARGS_HPP
#define ARGS_HPP
#include "types.hpp"


namespace Tools {
struct Args {
    static Vector<StringView> positionals;
    static StringView output;
    static void parse(char **argv);
};
}


#endif  // ARGS_HPP
