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
    const auto inputFiles = Tools::Args::positionals;
    const auto outputFile = Tools::Args::output;


    const auto inputProgram = Tools::loadFile(inputFiles.front());
    info("input = \n{}", inputProgram);

    Lex::Lexer lex {inputProgram};
    const auto tokens = lex.parseAll();

    Parse::Parser parse {tokens};
    const auto ast = parse.makeProgram();


    Comp::Compiler comp {ast};
    const auto outputCode = comp.compile();

    Tools::saveFile(outputCode, outputFile);

    return 0;
}
