#ifndef PARSER_INTERFACE_HPP
#define PARSER_INTERFACE_HPP
#include <FlexLexer.h>
#include <istream>
#include <optional>
#include <parser.hpp>

struct ParserInterface {
    static std::optional<yyFlexLexer> lexer;
    scotch::parser parser;
    friend int yylex(scotch::parser::semantic_type *yylval, scotch::parser::location_type *yylloc);
    ParserInterface(std::istream *input);
    int parse();
};



#endif  // PARSER_INTERFACE_HPP
