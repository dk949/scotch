#include "args.hpp"
#include "ast.hpp"
#include "ast_compiler.hpp"

#include <cstdlib>
#include <FlexLexer.h>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <parser.hpp>
#include <vector>

yyFlexLexer *lexer;

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

    lexer = new yyFlexLexer(&input);
    scotch::parser parser;
    parser.parse();
    delete lexer;
    return 0;
}
