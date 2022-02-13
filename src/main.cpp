#include "log.hpp"
log_init();

#include "args.hpp"
#include "ast.hpp"
#include "common.hpp"
#include "comp.hpp"
#include "file.hpp"
#include "parse.hpp"



dev const Lex::Builtins blt;
int main(int, char **argv) {
    ftrace();
    Tools::Args::parse(argv);
    Vector<StringView> positionals = Tools::Args::positionals;
    if (positionals.empty()) {
        crash("Expected file, got nothing");
    }
    String input = Tools::loadFile(positionals.back());
    info("input = \n{}", input);

    Lex::Lexer lex {input};
    const auto tokens = lex.parseAll();

    Parse::Parser parse {tokens};
    const auto ast = parse.makeProgram();


    Comp::Compiler comp {ast};
    const auto result = comp.compile();

    return 0;
}
