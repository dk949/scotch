#ifndef ARGS_HPP
#define ARGS_HPP
#include "class_tools.hpp"
#include "types.hpp"


namespace Tools {
struct Args {
public:
    enum class Verbosity { NONE, SOME, MOST, ALL };
    using enum Verbosity;

private:
    Args(Vector<StringView> positionals, StringView output, Verbosity verb, bool tok, bool ast, bool wat);
    Vector<StringView> m_positionals;
    StringView m_output;
    Verbosity m_verbosity;
    bool m_dumpTokens;
    bool m_dumpAst;
    bool m_dumpWat;

public:

    static Args parse(char **argv);

    GetProperty(positionals);
    GetProperty(output);
    GetProperty(verbosity);
    GetProperty(dumpTokens);
    GetProperty(dumpAst);
    GetProperty(dumpWat);
};
}  // namespace Tools


#endif  // ARGS_HPP
