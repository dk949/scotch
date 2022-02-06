#ifndef BUILTINS_HPP
#define BUILTINS_HPP
#include "token.hpp"
#include "types.hpp"

Map<StringView, Token::Keyword> kwMap {
    { "const",  Token::CONST},
    {   "def",    Token::DEF},
    {  "main",   Token::MAIN},
    {"return", Token::RETURN},
};

namespace {
dev static auto kwCheck = []() {
    verify(kwMap.size() == to<size_t>(Token::KW_COUNT));
    return 0;
}();
}

Map<StringView, Token::BuiltinType> typeMap {
    {"int", Token::INT},
};

namespace {
dev static auto typeCheck = []() {
    verify(typeMap.size() == to<size_t>(Token::TYPE_COUNT));
    return 0;
}();
}


Map<StringView, Token::Operator> opMap {
    {"+",      Token::PLUS},
    {"-",     Token::MINUS},
    {":",     Token::COLON},
    {";", Token::SEMICOLON},
    {"{",    Token::LCURLY},
    {"}",    Token::RCURLY},
};

bool isopstart(char c) {
    for (dev const auto &op : opMap) {
        if (c == op.first[0]) {
            return true;
        }
    }
    return false;
}

namespace {
dev static auto opCheck = []() {
    verify(opMap.size() == to<size_t>(Token::OP_COUNT));
    return 0;
}();
}



#endif  // BUILTINS_HPP
