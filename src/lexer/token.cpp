#include "token.hpp"

#include <string>

template<>
Int64 Token::get<Int64>() const {
    if (m_type == TokenType::T_INT) {
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
const char *Token::get<const char *>() const {
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
        : m_type(TokenType::T_INT) {
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

Token::Token(const char *val)
        : m_type(TokenType::T_IDENTIFIER) {
    const auto len = std::char_traits<char>::length(val) + 1;
    m_val.id = new char[len]();
    std::char_traits<char>::copy(m_val.id, val, len);
}

Token::Token(BuiltinType val)
        : m_type(TokenType::T_BUILTIN_TYPE) {
    m_val.type = val;
}
Token::Token()
        : m_type(TokenType::T_EOF) { }


Token::Token(const Token &other)
        : m_type(other.m_type) {
    if (other.m_type == TokenType::T_IDENTIFIER) {
        const auto len = std::char_traits<char>::length(other.m_val.id) + 1;  // + null terminator
        m_val.id = new char[len]();
        std::char_traits<char>::copy(m_val.id, other.m_val.id, len);
        return;
    }
    m_val = other.m_val;
}

Token &Token::operator=(const Token &other) {
    if (&other == this) {
        return *this;
    }

    m_type = other.m_type;
    if (other.m_type == TokenType::T_IDENTIFIER) {
        delete[] m_val.id;
        const auto len = std::char_traits<char>::length(other.m_val.id) + 1;
        auto *str = new char[len]();
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
        m_val = other.m_val;
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

Token::Order Token::operator<=>(Int64 i) const {
    if (m_type == TokenType::T_INT)
        return m_val.num <=> i;
    return Order::unordered;
}

Token::Order Token::operator<=>(Keyword k) const {
    if (m_type == TokenType::T_KEYWORD)
        return m_val.kw <=> k;
    return Order::unordered;
}

Token::Order Token::operator<=>(Operator o) const {
    if (m_type == TokenType::T_OP)
        return m_val.op <=> o;
    return Order::unordered;
}

Token::Order Token::operator<=>(StringView id) const {
    if (m_type == TokenType::T_IDENTIFIER)
        return StringView {m_val.id} <=> id;
    return Order::unordered;
}

Token::Order Token::operator<=>(BuiltinType t) const {
    if (m_type == TokenType::T_BUILTIN_TYPE)
        return t <=> m_val.type;
    return Order::unordered;
}

Token::Order Token::operator<=>(const Token &t) const {
    if (m_type != t.m_type)
        return Order::unordered;

    switch (t.m_type) {
        case (TokenType::T_BUILTIN_TYPE):
            return *this <=> t.m_val.type;
        case (TokenType::T_OP):
            return *this <=> t.m_val.op;
        case (TokenType::T_EOF):
            return Order::equivalent;
        case (TokenType::T_INT):
            return *this <=> t.m_val.num;
        case (TokenType::T_STR):
            todo();
        case (TokenType::T_KEYWORD):
            return *this <=> t.m_val.kw;
        case (TokenType::T_IDENTIFIER):
            return *this <=> t.m_val.id;
    }
    return Order::unordered;
}

bool Token::operator==(Int64 i) const {
    return (m_type == TokenType::T_INT) && m_val.num == i;
}

bool Token::operator==(Keyword k) const {
    return (m_type == TokenType::T_KEYWORD) && m_val.kw == k;
}

bool Token::operator==(Operator o) const {
    return (m_type == TokenType::T_OP) && m_val.op == o;
}

bool Token::operator==(StringView id) const {
    return (m_type == TokenType::T_IDENTIFIER) && StringView {m_val.id} == id;
}

bool Token::operator==(BuiltinType t) const {
    return (m_type == TokenType::T_BUILTIN_TYPE) && t == m_val.type;
}

bool Token::operator==(const Token &t) const {
    if (m_type != t.m_type)
        return false;

    switch (t.m_type) {
        case (TokenType::T_BUILTIN_TYPE):
            return *this == t.m_val.type;
        case (TokenType::T_OP):
            return *this == t.m_val.op;
        case (TokenType::T_EOF):
            return true;
        case (TokenType::T_INT):
            return *this == t.m_val.num;
        case (TokenType::T_STR):
            todo();
        case (TokenType::T_KEYWORD):
            return *this == t.m_val.kw;
        case (TokenType::T_IDENTIFIER):
            return *this == t.m_val.id;
    }
    return false;
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
        ENUM_CASE(Token::LBRACKET);
        ENUM_CASE(Token::RBRACKET);
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
