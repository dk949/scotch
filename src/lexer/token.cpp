#include "token.hpp"

template<>
Int64 Token::get<Int64>() const {
    if (m_type == TokenType::T_NUM) {
        return m_val.num;
    }
    crash("cannot extract Int64 value of token type {}", m_type);
}

template<>
Token::Keyword Token::get<Token::Keyword>() const {
    [[likely]] if (m_type == TokenType::T_KEYWORD) {
        return m_val.kw;
    }
    crash("cannot extract Keyword value of token type {}", m_type);
}

template<>
Token::Operator Token::get<Token::Operator>() const {
    [[likely]] if (m_type == TokenType::T_OP) {
        return m_val.op;
    }
    crash("cannot extract Operator value of token type {}", m_type);
}


template<>
Token::IdentifierHashT Token::get<Token::IdentifierHashT>() const {
    [[likely]] if (m_type == TokenType::T_IDENTIFIER) {
        return m_val.id;
    }
    crash("cannot extract Identifier value of token type {}", m_type);
}

template<>
Token::BuiltinType Token::get<Token::BuiltinType>() const {
    [[likely]] if (m_type == TokenType::T_BUILTIN_TYPE) {
        return m_val.type;
    }
    crash("cannot extract BuiltinType value of token type {}", m_type);
}



template<>
void Token::get<void>() const {
    [[likely]] if (m_type == TokenType::T_EOF) {
        return;
    }
    crash("cannot extract Identifier value of token type {}", m_type);
}


Token::Token(Int64 val)
        : m_type(TokenType::T_NUM) {
    m_val.num = val;
}
Token::Token(Keyword val)
        : m_type(TokenType::T_KEYWORD) {
    m_val.kw = val;
}
Token::Token(Operator val)
        : m_type(TokenType::T_OP) {
    m_val.op = val;
}
Token::Token(IdentifierHashT val)
        : m_type(TokenType::T_IDENTIFIER) {
    m_val.id = val;
}
Token::Token(BuiltinType val)
        : m_type(TokenType::T_BUILTIN_TYPE) {
    m_val.type = val;
}
Token::Token()
        : m_type(TokenType::T_EOF) { }


bool Token::isEOF() const {
    return m_type == TokenType::T_EOF;
}

TokenType Token::type() const {
    return m_type;
}



#undef ENUM_DO
#define ENUM_DO(E) return #E

const char *Token::opToStr(Token::Operator b) {
    switch (b) {
        ENUM_CASE(Token::MINUS);
        ENUM_CASE(Token::PLUS);
        ENUM_CASE(Token::COLON);
        ENUM_CASE(Token::SEMICOLON);
        ENUM_CASE(Token::LCURLY);
        ENUM_CASE(Token::RCURLY);
        BAD_ENUM_CASE(Token::OP_COUNT);
    }
    crash("Unsuppirted operator {}", b);
}
const char *Token::kwToStr(Token::Keyword k) {
    switch (k) {
        ENUM_CASE(Token::CONST);
        ENUM_CASE(Token::DEF);
        ENUM_CASE(Token::MAIN);
        ENUM_CASE(Token::RETURN);
        BAD_ENUM_CASE(Token::KW_COUNT);
    }
    crash("Unsuppirted keyword {}", k);
};

const char *Token::typeToStr(Token::BuiltinType t) {
    switch (t) {
        ENUM_CASE(Token::INT);
        BAD_ENUM_CASE(Token::TYPE_COUNT);
    }
    crash("Unsuppirted keyword {}", t);
};
