#include "args.hpp"

#include "ftrace.hpp"
#include "log.hpp"
#include "switch_tools.hpp"

#include <vector>


namespace Tools {

Args::Args(Vector<StringView> positionals, StringView output, Verbosity verb, bool tok, bool ast, bool wat)
        : m_positionals(std::move(positionals))
        , m_output(output)
        , m_verbosity(verb)
        , m_dumpTokens(tok)
        , m_dumpAst(ast)
        , m_dumpWat(wat) {
    ftrace();
}
#define Do(Action)            \
    do                        \
        Action while (false); \
    continue


void help(StringView prog) {
    fmt::print(R"(usage: {} [OPTIONS] FILE

compile source .scot file to WebAssembly (text format)

positional arguments:
  FILE                          input file to compile

options:
    -h, --help                  display this message and exit
    -V, --version               display version information and exit

    -o,--output                 name of the output file. Default: output.wat

    -v,--verbose                print some debug information
    -vv,--very-verbose          print most debug information
    -vvv, --very-very-verbose   print all debug information


    --dump-tokens               print tokens of the input file
    --dump-ast                  print ast created from the tokens
    --dump-wat                  print webassembly created from the ast
)",
        Tools::filename(prog));

    exit(0);
}



void version(StringView prog) {
    fmt::print("{}: {}",
        Tools::filename(prog),
#ifdef SC_VERSION
        "v" SC_VERSION "\n"
#else
        "version unavailable."
#endif
    );
    exit(0);
}


Args Args::parse(char **argv) {
    ftrace();
    auto *prog = *argv;
    StringView output;
    Vector<StringView> positionals;
    Verbosity verb = NONE;
    bool tok = false;
    bool ast = false;
    bool wat = false;

    for (const char *arg = *(++argv); arg; arg = *(++argv)) {
        // clang-format off
        str_switch(arg) {

            str_case("--version"):
            str_case("-V"):
                Do(version(prog););

            str_case("-h"):
            str_case("--help"):
                Do(help(prog););

            str_case("-o"):
            str_case("--output"):
                Do({
                    if(*(argv++)) {
                        output = *argv;
                    } else {
                        fmt::print("Expected filename after '-o', got nothing\n");
                        exit(1);
                    }
                });

            str_case("--verbose"):
            str_case("-v"):
                Do(verb = Verbosity::SOME;);

            str_case("--very-verbose"):
            str_case("-vv"):
                Do(verb = Verbosity::MOST;);

            str_case("--very-very-verbose"):
            str_case("-vvv"):
                Do(verb = Verbosity::ALL;);

            str_case("--dump-tokens"):
                Do(tok = true;);

            str_case("--dump-ast"):
                Do(ast = true;);

            str_case("--dump-wat"):
                Do(wat = true;);
        }
        // clang-format on
        positionals.emplace_back(arg);
    }

    if (positionals.empty()) {
        fmt::print("Expected file, got nothing\n");
        exit(1);
    }
    if (output.empty()) {
        output = "output.wat";
    }
    return Args(positionals, output, verb, tok, ast, wat);
}


}  // namespace Tools
