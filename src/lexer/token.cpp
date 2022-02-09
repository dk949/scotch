#include "token.hpp"

#include <string>

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
Token::IdentifierPtr Token::get<Token::IdentifierPtr>() const {
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

Token::Token(IdentifierPtr val)
        : m_type(TokenType::T_IDENTIFIER) {
    const auto len = std::char_traits<char>::length(val);
    auto *str      = new char[len]();
    std::char_traits<char>::copy(str, val, len);
    m_val.id = str;
}

Token::Token(BuiltinType val)
        : m_type(TokenType::T_BUILTIN_TYPE) {
    m_val.type = val;
}
Token::Token()
        : m_type(TokenType::T_EOF) { }


Token::Token(const Token &other)
        : m_type(other.m_type)
        , m_val(other.m_val) {
    debug("Copy construction: token");
    if (other.m_type == TokenType::T_IDENTIFIER) {
        const auto len = std::char_traits<char>::length(other.m_val.id);
        auto *str      = new char[len]();
        std::char_traits<char>::copy(str, other.m_val.id, len);
        m_val.id = str;
    }
}

Token &Token::operator=(const Token &other) {
    if (&other == this) {
        return *this;
    }

    debug("Copy assignment: token");
    m_type = other.m_type;
    if (other.m_type == TokenType::T_IDENTIFIER) {
        delete[] m_val.id;
        const auto len = std::char_traits<char>::length(other.m_val.id);
        auto *str      = new char[len]();
        std::char_traits<char>::copy(str, other.m_val.id, len);
        m_val.id = str;
        return *this;
    }

    m_val = other.m_val;

    return *this;
}

Token::Token(Token &&other)
        : m_type(other.m_type)
        , m_val(other.m_val) {
    if (other.m_type == TokenType::T_IDENTIFIER) {
        other.m_val.id = nullptr;
    }
}

Token &Token::operator=(Token &&other) {
    if (&other == this) {
        return *this;
    }

    m_type = other.m_type;

    if (other.m_type == TokenType::T_IDENTIFIER) {
        delete[] m_val.id;
        m_val          = other.m_val;
        other.m_val.id = nullptr;
        return *this;
    }
    m_val = other.m_val;
    return *this;
}

Token::~Token() {
    if (m_type == TokenType::T_IDENTIFIER) {
        delete[] m_val.id;
    }
}


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
