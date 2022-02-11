#include "builtins.hpp"

#include "common.hpp"
#include "ftrace.hpp"
#include "log.hpp"

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
    {"(",  Token::LBRACKET},
    {")",  Token::RBRACKET},
};

Builtins::Builtins() {
    ftrace();
    if (m_i) {
        crash("{}",
            "Builtins needs to be instantiated exactly once,"
            " before entering main");
    }
    verify(kwMap.size() == to<size_t>(Token::KW_COUNT));
    verify(opMap.size() == to<size_t>(Token::OP_COUNT));
    verify(typeMap.size() == to<size_t>(Token::TYPE_COUNT));
    m_i = true;
}

bool Builtins::isopstart(char c) {
    ftrace();
    for (const auto &op : opMap) {
        if (c == op.first[0]) {
            return true;
        }
    }
    return false;
}
