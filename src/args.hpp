#ifndef ARGS_HPP
#define ARGS_HPP
#include "macros.hpp"

#include <string_view>
#include <vector>


namespace scotch {
struct Args {
public:
    enum class Verbosity { NONE, SOME, MOST, ALL };
    using enum Verbosity;

private:
    Args(std::vector<std::string_view> positionals, std::string_view output, Verbosity verb, bool tok, bool ast, bool wat);

    std::vector<std::string_view> PropertyGet(positionals)
    std::string_view PropertyGet(output)
    Verbosity PropertyGet(verbosity)
    bool PropertyGet(dumpTokens)
    bool PropertyGet(dumpAst)
    bool PropertyGet(dumpWat)

public:

    static Args parse(char **argv);
};
}  // namespace scotch


#endif  // ARGS_HPP
