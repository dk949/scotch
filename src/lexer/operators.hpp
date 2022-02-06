#ifndef OPERATORS_HPP
#define OPERATORS_HPP

#include "token.hpp"
#include "types.hpp"


// FIXME: no way of handleling multichar operators

// { PLUS, MINUS, COLON, SEMICOLON, LCURLY, RCURLY }
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



#endif  // OPERATORS_HPP
