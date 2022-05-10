#include "args.hpp"

#include "common.hpp"

#include <project/config.hpp>
#include <vector>


namespace scotch {

Args::Args(std::vector<std::string_view> positionals, std::string_view output, Verbosity verb, bool tok, bool ast, bool wat)
        : m_positionals(std::move(positionals))
        , m_output(output)
        , m_verbosity(verb)
        , m_dumpTokens(tok)
        , m_dumpAst(ast)
        , m_dumpWat(wat) { }


void help() {
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
        project::name);
    exit(0);
}



void version() {
    fmt::print("{}: v{}", project::name, project::version::full);
    exit(0);
}

#define Do(Action)            \
    do                        \
        Action while (false); \
    continue

Args Args::parse(char **argv) {
    std::string_view output;
    std::vector<std::string_view> positionals;
    Verbosity verb = NONE;
    bool tok = false;
    bool ast = false;
    bool wat = false;

    for (const char *arg = *(++argv); arg; arg = *(++argv)) {
        // clang-format off
        switch(scotch::hash(arg)) {

            case scotch::hash("--version"):
            case scotch::hash("-V"):
                Do(version(););

            case scotch::hash("-h"):
            case scotch::hash("--help"):
                Do(help(););

            case scotch::hash("-o"):
            case scotch::hash("--output"):
                Do({
                    if(*(argv++)) {
                        output = *argv;
                    } else {
                        fmt::print("Expected filename after '-o', got nothing\n");
                        exit(1);
                    }
                });

            case scotch::hash("--verbose"):
            case scotch::hash("-v"):
                Do(verb = Verbosity::SOME;);

            case scotch::hash("--very-verbose"):
            case scotch::hash("-vv"):
                Do(verb = Verbosity::MOST;);

            case scotch::hash("--very-very-verbose"):
            case scotch::hash("-vvv"):
                Do(verb = Verbosity::ALL;);

            case scotch::hash("--dump-tokens"):
                Do(tok = true;);

            case scotch::hash("--dump-ast"):
                Do(ast = true;);

            case scotch::hash("--dump-wat"):
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

}  // namespace scotch
