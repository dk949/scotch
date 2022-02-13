#include "token.hpp"

#include "common.hpp"
#include "ftrace.hpp"
#include "log.hpp"


template<>
Int64 Lex::Token::get<Int64>() const {
    ftrace();
    if (m_type == Lex::TokenType::T_INT) {
        return m_val.num;
    }
    crash("cannot extract Int64 value of token type {}", m_type);
}

template<>
Lex::Token::Keyword Lex::Token::get<Lex::Token::Keyword>() const {
    ftrace();
    if (m_type == Lex::TokenType::T_KEYWORD) [[likely]] {
        return m_val.kw;
    }
    crash("cannot extract Keyword value of token type {}", m_type);
}

template<>
Lex::Token::Operator Lex::Token::get<Lex::Token::Operator>() const {
    ftrace();
    if (m_type == Lex::TokenType::T_OP) [[likely]] {
        return m_val.op;
    }
    crash("cannot extract Operator value of token type {}", m_type);
}


template<>
const char *Lex::Token::get<const char *>() const {
    ftrace();
    if (m_type == Lex::TokenType::T_IDENTIFIER) [[likely]] {
        return m_val.id;
    }
    crash("cannot extract Identifier value of token type {}", m_type);
}

template<>
Lex::Token::BuiltinType Lex::Token::get<Lex::Token::BuiltinType>() const {
    ftrace();
    if (m_type == Lex::TokenType::T_BUILTIN_TYPE) [[likely]] {
        return m_val.type;
    }
    crash("cannot extract BuiltinType value of token type {}", m_type);
}



template<>
void Lex::Token::get<void>() const {
    ftrace();
    if (m_type == Lex::TokenType::T_EOF) [[likely]] {
        return;
    }
    crash("cannot extract Identifier value of token type {}", m_type);
}


Lex::Token::Token(Int64 val)
        : m_type(Lex::TokenType::T_INT) {
    m_val.num = val;
}
Lex::Token::Token(Keyword val)
        : m_type(Lex::TokenType::T_KEYWORD) {
    ftrace();
    m_val.kw = val;
}
Lex::Token::Token(Operator val)
        : m_type(Lex::TokenType::T_OP) {
    ftrace();
    m_val.op = val;
}

Lex::Token::Token(const char *val)
        : m_type(Lex::TokenType::T_IDENTIFIER) {
    ftrace();
    const auto len = std::char_traits<char>::length(val) + 1;
    m_val.id = new char[len]();
    std::char_traits<char>::copy(m_val.id, val, len);
}

Lex::Token::Token(BuiltinType val)
        : m_type(Lex::TokenType::T_BUILTIN_TYPE) {
    ftrace();
    m_val.type = val;
}
Lex::Token::Token()
        : m_type(Lex::TokenType::T_EOF) {
    ftrace();
}


Lex::Token::Token(const Token &other)
        : m_type(other.m_type) {
    ftrace();
    if (other.m_type == Lex::TokenType::T_IDENTIFIER) {
        const auto len = std::char_traits<char>::length(other.m_val.id) + 1;  // + null terminator
        m_val.id = new char[len]();
        std::char_traits<char>::copy(m_val.id, other.m_val.id, len);
        return;
    }
    m_val = other.m_val;
}

Lex::Token &Lex::Token::operator=(const Token &other) {
    ftrace();
    if (&other == this) {
        return *this;
    }

    m_type = other.m_type;
    if (other.m_type == Lex::TokenType::T_IDENTIFIER) {
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

Lex::Token::Token(Token &&other) noexcept
        : m_type(other.m_type)
        , m_val(other.m_val) {
    ftrace();
    if (other.m_type == Lex::TokenType::T_IDENTIFIER) {
        other.m_val.id = nullptr;
    }
}

Lex::Token &Lex::Token::operator=(Token &&other) noexcept {
    ftrace();
    if (&other == this) {
        return *this;
    }

    m_type = other.m_type;

    if (other.m_type == Lex::TokenType::T_IDENTIFIER) {
        delete[] m_val.id;
        m_val = other.m_val;
        other.m_val.id = nullptr;
        return *this;
    }
    m_val = other.m_val;
    return *this;
}

Lex::Token::~Token() {
    ftrace();
    if (m_type == Lex::TokenType::T_IDENTIFIER) {
        delete[] m_val.id;
    }
}


bool Lex::Token::isEOF() const {
    ftrace();
    return m_type == Lex::TokenType::T_EOF;
}

bool Lex::Token::isBinExpr() const {
    ftrace();
    if (m_type != Lex::TokenType::T_OP) {
        return false;
    }
    // clang-format off
    switch (m_val.op) {
        case PLUS:
        case MINUS:
            return true;
        case COLON:
        case SEMICOLON:
        case LCURLY:
        case RCURLY:
        case LBRACKET:
        case RBRACKET:
            return false;
        BAD_ENUM_CASE(OP_COUNT);
    }
    // clang-format on
    unreachable("exhaustive enum");
    // return false;
}

Lex::Token::Order Lex::Token::operator<=>(Int64 i) const {
    ftrace();
    if (m_type == Lex::TokenType::T_INT) {
        return m_val.num <=> i;
    }
    return Order::unordered;
}

Lex::Token::Order Lex::Token::operator<=>(Keyword k) const {
    ftrace();
    if (m_type == Lex::TokenType::T_KEYWORD) {
        return m_val.kw <=> k;
    }
    return Order::unordered;
}

Lex::Token::Order Lex::Token::operator<=>(Operator o) const {
    ftrace();
    if (m_type == Lex::TokenType::T_OP) {
        return m_val.op <=> o;
    }
    return Order::unordered;
}

Lex::Token::Order Lex::Token::operator<=>(StringView id) const {
    ftrace();
    if (m_type == Lex::TokenType::T_IDENTIFIER) {
        return StringView {m_val.id} <=> id;
    }
    return Order::unordered;
}

Lex::Token::Order Lex::Token::operator<=>(BuiltinType t) const {
    ftrace();
    if (m_type == Lex::TokenType::T_BUILTIN_TYPE) {
        return t <=> m_val.type;
    }
    return Order::unordered;
}

Lex::Token::Order Lex::Token::operator<=>(const Token &t) const {
    ftrace();
    if (m_type != t.m_type) {
        return Order::unordered;
    }

    switch (t.m_type) {
        case (Lex::TokenType::T_BUILTIN_TYPE):
            return *this <=> t.m_val.type;
        case (Lex::TokenType::T_OP):
            return *this <=> t.m_val.op;
        case (Lex::TokenType::T_EOF):
            return Order::equivalent;
        case (Lex::TokenType::T_INT):
            return *this <=> t.m_val.num;
        case (Lex::TokenType::T_STR):
            fixme("{}", "string literals are not being handled");
            todo();
        case (Lex::TokenType::T_KEYWORD):
            return *this <=> t.m_val.kw;
        case (Lex::TokenType::T_IDENTIFIER):
            return *this <=> t.m_val.id;
    }
    return Order::unordered;
}

bool Lex::Token::operator==(Int64 i) const {
    ftrace();
    return (m_type == Lex::TokenType::T_INT) && m_val.num == i;
}

bool Lex::Token::operator==(Keyword k) const {
    ftrace();
    return (m_type == Lex::TokenType::T_KEYWORD) && m_val.kw == k;
}

bool Lex::Token::operator==(Operator o) const {
    ftrace();
    return (m_type == Lex::TokenType::T_OP) && m_val.op == o;
}

bool Lex::Token::operator==(StringView id) const {
    ftrace();
    return (m_type == Lex::TokenType::T_IDENTIFIER) && StringView {m_val.id} == id;
}

bool Lex::Token::operator==(BuiltinType t) const {
    ftrace();
    return (m_type == Lex::TokenType::T_BUILTIN_TYPE) && t == m_val.type;
}

bool Lex::Token::operator==(const Token &t) const {
    ftrace();
    if (m_type != t.m_type) {
        return false;
    }

    switch (t.m_type) {
        case (Lex::TokenType::T_BUILTIN_TYPE):
            return *this == t.m_val.type;
        case (Lex::TokenType::T_OP):
            return *this == t.m_val.op;
        case (Lex::TokenType::T_EOF):
            return true;
        case (Lex::TokenType::T_INT):
            return *this == t.m_val.num;
        case (Lex::TokenType::T_STR):
            fixme("{}", "string literals are not being handled");
            todo();
        case (Lex::TokenType::T_KEYWORD):
            return *this == t.m_val.kw;
        case (Lex::TokenType::T_IDENTIFIER):
            return *this == t.m_val.id;
    }
    return false;
}



Lex::TokenType Lex::Token::type() const {
    ftrace();
    return m_type;
}



#undef ENUM_DO
#define ENUM_DO(E) return Tools::shortEnumName(#E);

StringView Lex::Token::opToStr(Token::Operator b) {
    ftrace();
    switch (b) {
        ENUM_CASE(Lex::Token::MINUS);
        ENUM_CASE(Lex::Token::PLUS);
        ENUM_CASE(Lex::Token::COLON);
        ENUM_CASE(Lex::Token::SEMICOLON);
        ENUM_CASE(Lex::Token::LCURLY);
        ENUM_CASE(Lex::Token::RCURLY);
        ENUM_CASE(Lex::Token::LBRACKET);
        ENUM_CASE(Lex::Token::RBRACKET);
        BAD_ENUM_CASE(Lex::Token::OP_COUNT);
    }
    crash("Unsupported operator {}", b);
}
StringView Lex::Token::kwToStr(Token::Keyword k) {
    ftrace();
    switch (k) {
        ENUM_CASE(Lex::Token::CONST);
        ENUM_CASE(Lex::Token::DEF);
        ENUM_CASE(Lex::Token::RETURN);
        BAD_ENUM_CASE(Lex::Token::KW_COUNT);
    }
    crash("Unsupported keyword {}", k);
};

StringView Lex::Token::typeToStr(Token::BuiltinType t) {
    ftrace();
    switch (t) {
        ENUM_CASE(Lex::Token::INT);
        BAD_ENUM_CASE(Lex::Token::TYPE_COUNT);
    }
    crash("Unsupported type {}", t);
};
