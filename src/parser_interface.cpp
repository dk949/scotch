#include "parser_interface.hpp"

#include <parser.hpp>

void scotch::parser::error(const location_type &loc, const std::string &msg) {
    std::cerr << loc.begin.line << ':' << loc.begin.column << ':' << msg << '\n';
    //exit(1);
}

std::optional<yyFlexLexer> ParserInterface::lexer = std::nullopt;
ParserInterface::ParserInterface(std::istream *input)
        : parser() {
    lexer.emplace(input);
}

int ParserInterface::parse() {
    return parser.parse();
}

int yylex(scotch::parser::semantic_type *yylval, scotch::parser::location_type *yylloc) {
    yylloc->begin.line = ParserInterface::lexer->lineno();
    int token = ParserInterface::lexer->yylex();
    switch (token) {
        case scotch::parser::token::IDENT:
            yylval->build(std::string(ParserInterface::lexer->YYText()));
            break;
        case scotch::parser::token::Int32:
        case scotch::parser::token::Int64:
            yylval->build(std::strtoll(ParserInterface::lexer->YYText(), nullptr, 10));
            break;
        case scotch::parser::token::Float32:
        case scotch::parser::token::Float64:
            yylval->build(std::strtod(ParserInterface::lexer->YYText(), nullptr));
            break;
    }
    return token;
}
