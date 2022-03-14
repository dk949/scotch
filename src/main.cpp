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
    switch (args.verbosity()) {
        case Tools::Args::NONE:
            spdlog::set_level(spdlog::level::warn);
            break;
        case Tools::Args::SOME:
            spdlog::set_level(spdlog::level::info);
            break;
        case Tools::Args::MOST:
            spdlog::set_level(spdlog::level::debug);
            break;
        case Tools::Args::ALL:
            spdlog::set_level(spdlog::level::trace);
            break;
    }
    const auto inputFiles = args.positionals();
    const auto outputFile = args.output();

    Tools::File::readFile(inputFiles.front())
        .map([&args](const auto &input) {
            spdlog::info("Begin compilation");


            Lex::Lexer lex {input};
            const auto tokens = lex.parseAll();
            if (args.dumpTokens())
                spdlog::debug("Tokens:\n{}", fmt::join(tokens, ",\n"));

            spdlog::info("Lexing complete");

            Parse::Parser parse {tokens};
            const auto ast = parse.makeProgram();
            if (args.dumpAst())
                spdlog::debug("AST:\n{}", to<Ast::NodePtr>(ast));

            spdlog::info("Parsing complete");

            Comp::Compiler comp {ast};
            const auto outputCode = comp.compile();
            if (args.dumpWat())
                spdlog::debug("Output:\n{}", outputCode);

            spdlog::info("Compilation complete");

            return outputCode;
        })
        .and_then([&outputFile](const auto &output) { return Tools::File::writeFile(outputFile, output); })
        .map_error([](const auto &fileError) {
            crash("Failed file operation: {}: {}", fileError.errStr, std::strerror(fileError.errc));
        });
    return 0;
}
