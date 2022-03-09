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
            spdlog::debug("Begin compilation");
            Lex::Lexer lex {input};
            const auto tokens = lex.parseAll();
            spdlog::debug("Lexing complete");

            Parse::Parser parse {tokens};
            const auto ast = parse.makeProgram();
            spdlog::debug("Parsing complete");


            Comp::Compiler comp {ast};
            const auto outputCode = comp.compile();
            spdlog::debug("Compilation complete");

            return outputCode;
        })
        .and_then([&outputFile](const auto &output) { return Tools::File::writeFile(outputFile, output); })
        .map_error([](const auto &fileError) {
            crash("Failed file operation: {}: {}", fileError.errStr, std::strerror(fileError.errc));
        });
    return 0;
}
