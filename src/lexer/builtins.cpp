#include "builtins.hpp"

#include "common.hpp"
#include "ftrace.hpp"
#include "log.hpp"

bool Lex::Builtins::m_i = false;

const Map<StringView, Lex::Token::Keyword> Lex::Builtins::kwMap {
    { "const",  Lex::Token::CONST},
    {   "def",    Lex::Token::DEF},
    {"return", Lex::Token::RETURN},
};

const Map<StringView, Lex::Token::BuiltinType> Lex::Builtins::typeMap {
    {"i32", Lex::Token::I32},
    {"i64", Lex::Token::I64},
    {"f32", Lex::Token::F32},
    {"f64", Lex::Token::F64},
};

const Map<StringView, Lex::Token::Operator> Lex::Builtins::opMap {
    {"+",      Lex::Token::PLUS},
    {"-",     Lex::Token::MINUS},
    {":",     Lex::Token::COLON},
    {";", Lex::Token::SEMICOLON},
    {"{",    Lex::Token::LCURLY},
    {"}",    Lex::Token::RCURLY},
    {"(",  Lex::Token::LBRACKET},
    {")",  Lex::Token::RBRACKET},
};

Lex::Builtins::Builtins() {
    ftrace();
    if (m_i) {
        crash("Lex::Builtins needs to be instantiated exactly once,"
              "before entering main");
    }
    verify(kwMap.size() == to<size_t>(Lex::Token::KW_COUNT));
    verify(opMap.size() == to<size_t>(Lex::Token::OP_COUNT));
    verify(typeMap.size() == to<size_t>(Lex::Token::TYPE_COUNT));
    m_i = true;
}

bool Lex::Builtins::isopstart(char c) {
    ftrace();
    for (const auto &op : opMap) {
        if (c == op.first[0]) {
            return true;
        }
    }
    return false;
}
