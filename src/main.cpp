#include "args.hpp"
#include "ast.hpp"
#include "ast_compiler.hpp"
#include "common.hpp"
#include "console_error.hpp"
#include "console_out.hpp"
#include "empty_post_proc.hpp"
#include "empty_pre_proc.hpp"
#include "parser_options.hpp"
#include "wasm_compiler.hpp"
#include "wasm_formatter.hpp"

#include <cstdlib>
#include <FlexLexer.h>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <numeric>
#include <optional>
#include <parser.hpp>
#include <vector>

std::optional<yyFlexLexer> lexer;

int yylex(scotch::parser::semantic_type *yylval, scotch::parser::location_type *yylloc) {
    yylloc->begin.line = lexer->lineno();
    int token = lexer->yylex();
    switch (token) {
        case scotch::parser::token::IDENT:
            yylval->build(std::string(lexer->YYText()));
            break;
        case scotch::parser::token::Int32:
        case scotch::parser::token::Int64:
            yylval->build(std::strtoll(lexer->YYText(), nullptr, 10));
            break;
        case scotch::parser::token::Float32:
        case scotch::parser::token::Float64:
            yylval->build(std::strtod(lexer->YYText(), nullptr));
            break;
    }
    return token;
}

void scotch::parser::error(const location_type &loc, const std::string &msg) {
    std::cerr << loc.begin.line << ':' << loc.begin.column << ':' << msg << '\n';
    exit(1);
}



int main(int, char *argv[]) {
    const auto args = scotch::Args::parse(argv);

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

    Pipeline p {scotch::makeVector<std::unique_ptr<Preprocessor>>(std::make_unique<EmptyPreproc>()),
        std::move(compiler),
        scotch::makeVector<std::unique_ptr<Postprocessor>>(std::make_unique<WasmFormatter>()),
        std::make_unique<ConsoleOut>(),
        std::make_unique<ConsoleError>()};

    ParserOptions::setPipeline(std::move(p));

    lexer.emplace(&input);
    scotch::parser parser;
    parser.parse();
    return 0;
}
