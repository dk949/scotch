#include "builtins.hpp"

bool Builtins::m_i = false;

const Map<StringView, Token::Keyword> Builtins::kwMap {
    { "const",  Token::CONST},
    {   "def",    Token::DEF},
    {"return", Token::RETURN},
};

const Map<StringView, Token::BuiltinType> Builtins::typeMap {
    {"int", Token::INT},
};

const Map<StringView, Token::Operator> Builtins::opMap {
    {"+",      Token::PLUS},
    {"-",     Token::MINUS},
    {":",     Token::COLON},
    {";", Token::SEMICOLON},
    {"{",    Token::LCURLY},
    {"}",    Token::RCURLY},
};

bool Builtins::isopstart(char c) {
    for (dev const auto &op : opMap) {
        if (c == op.first[0]) {
            return true;
        }
    }
    return false;
}
