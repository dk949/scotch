%option c++ noyywrap yylineno outfile="lexer.cpp"

%{
#include <iostream>
#include <cstdlib>
#include <vector>
#include "ast.hpp"
#include "ast_compiler.hpp"
#include "parser.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#if (defined(__GNUC__) && !defined(__clang__))
#pragma GCC diagnostic ignored "-Wuseless-cast"
#endif

using namespace scotch;
%}

%%
"("                         return parser::token::LPAREN;
")"                         return parser::token::RPAREN;
"{"                         return parser::token::LCURLY;
"}"                         return parser::token::RCURLY;
":"                         return parser::token::COLON;
";"                         return parser::token::SEMICOLON;
","                         return parser::token::COMMA;
"="                         return parser::token::ASSIGN;
"=="                        return parser::token::EQUAL;
"+"                         return parser::token::PLUS;
"-"                         return parser::token::MINUS;
"*"                         return parser::token::TIMES;
"/"                         return parser::token::DIVIDE;
"%"                         return parser::token::MODULO;
"<"                         return parser::token::LESS;
">"                         return parser::token::GREATER;
"<="                        return parser::token::LESSTHENEQUAL;
">="                        return parser::token::GREATERTHENEQUAL;
"!"                         return parser::token::NOT;
"||"                        return parser::token::OR;
"&&"                        return parser::token::AND;
module                      return parser::token::MODULE;
def                         return parser::token::DEF;
return                      return parser::token::RETURN;
const                       return parser::token::CONST;
let                         return parser::token::LET;
true                        return parser::token::TRUE;
false                       return parser::token::FALSE;
if                          return parser::token::IF;
else                        return parser::token::ELSE;
[0-9]+i                     return parser::token::Int32;
[0-9]+l?                    return parser::token::Int64;
[0-9]+"."[0-9]+f            return parser::token::Float32;
[0-9]+"."[0-9]+d?           return parser::token::Float64;
[a-zA-Z_][a-zA-Z0-9_]*      return parser::token::IDENT;
[ \t\n]+                    // Whitespaces skipped.
"//".*                      // Commens skipped.
.                           { std::cerr << "Line " << lineno() << ": Unexpected character: '" << YYText() << "'." << std::endl; exit(1); }


%%
#pragma GCC diagnostic pop
