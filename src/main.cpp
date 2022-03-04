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

    const auto args = Tools::Args::parse(argv);
    const auto inputFiles = args.positionals();
    const auto outputFile = args.output();

    Tools::File::readFile(inputFiles.front())
        .map([](const auto &input) {
            Lex::Lexer lex {input};
            const auto tokens = lex.parseAll();

            Parse::Parser parse {tokens};
            const auto ast = parse.makeProgram();


            Comp::Compiler comp {ast};
            const auto outputCode = comp.compile();

            return outputCode;
        })
        .and_then([&outputFile](const auto &output) { return Tools::File::writeFile(outputFile, output); })
        .map_error([](const auto &fileError) {
            crash("Failed file operation: {}: {}", fileError.errStr, std::strerror(fileError.errc));
        });
    return 0;
}
