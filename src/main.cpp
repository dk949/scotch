#include "args.hpp"
#include "ast.hpp"
#include "ast_compiler.hpp"
#include "common.hpp"
#include "console_io.hpp"
#include "empty_post_proc.hpp"
#include "empty_pre_proc.hpp"
#include "io.hpp"
#include "parser_interface.hpp"
#include "parser_options.hpp"
#include "wasm_compiler.hpp"
#include "wasm_formatter.hpp"

#include <cstdlib>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <numeric>
#include <optional>
#include <vector>

std::optional<yyFlexLexer> lexer;


int main(int, char *argv[]) {
    const auto args = Args::parse(argv);

    std::ifstream input(args.positionals().front().data());
    if (!input) {
        fmt::print(stderr, "Cannot open input file: {}", args.positionals().front());
        exit(1);
    }

    std::unique_ptr<Compiler> compiler = nullptr;
    if (args.dumpAst())
        compiler = std::make_unique<AstCompiler>();
    else
        compiler = std::make_unique<WasmCompiler>();

    Pipeline p {
        scotch::makeVector<std::unique_ptr<Preprocessor>>(std::make_unique<EmptyPreproc>()),
        std::move(compiler),
        scotch::makeVector<std::unique_ptr<Postprocessor>>(std::make_unique<WasmFormatter>()),
        Io::makeIo<ConsoleOut, ConsoleError>()
    };

    ParserOptions::setPipeline(std::move(p));
    ParserInterface parser {&input};
    return parser.parse();
}
