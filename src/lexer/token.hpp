#ifndef TOKEN_HPP
#define TOKEN_HPP
#include "hash.hpp"
#include "token_type.hpp"
#include "tools.hpp"
#include "types.hpp"

struct Token {
public:
    using IdentifierHashT = uint64_t;

    enum class Keyword {
        CONST,
        DEF,
        MAIN,
        RETURN,


        KW_COUNT,
    };
    enum class Operator {
        PLUS,
        MINUS,
        COLON,
        SEMICOLON,
        LCURLY,
        RCURLY,



        OP_COUNT,
    };

    enum class BuiltinType {
        INT,

        TYPE_COUNT,
    };
    using enum Keyword;
    using enum Operator;
    using enum BuiltinType;

    union Value {
        Int64 num;
        Keyword kw;
        Operator op;
        BuiltinType type;
        IdentifierHashT id;
    };

private:
    TokenType m_type;

    Value m_val;

public:
    explicit Token(Int64 val)
            : m_type(TokenType::T_NUM) {
        m_val.num = val;
    }
    explicit Token(Keyword val)
            : m_type(TokenType::T_KEYWORD) {
        m_val.kw = val;
    }
    explicit Token(Operator val)
            : m_type(TokenType::T_OP) {
        m_val.op = val;
    }
    explicit Token(IdentifierHashT val)
            : m_type(TokenType::T_IDENTIFIER) {
        m_val.id = val;
    }
    explicit Token(BuiltinType val)
            : m_type(TokenType::T_BUILTIN_TYPE) {
        m_val.type = val;
    }
    explicit Token()
            : m_type(TokenType::T_EOF) { }


    bool isEOF() {
        return m_type == TokenType::T_EOF;
    }

    TokenType type() const {
        return m_type;
    }

    template<typename T>
    T get() const {
        static_assert(sizeof(T) < 0, "get used with an invalid type");
    }



    constexpr static IdentifierHashT hash(std::string_view sv) {
        return CTHash::fnv_1a<IdentifierHashT>(sv);
    }

#undef ENUM_DO
#define ENUM_DO(E) return #E
#define FOEREACH_OP(B)                  \
    switch (B) {                        \
        ENUM_CASE(Token::MINUS);        \
        ENUM_CASE(Token::PLUS);         \
        ENUM_CASE(Token::COLON);        \
        ENUM_CASE(Token::SEMICOLON);    \
        ENUM_CASE(Token::LCURLY);       \
        ENUM_CASE(Token::RCURLY);       \
        BAD_ENUM_CASE(Token::OP_COUNT); \
    }

#define FOEREACH_KEYWORD(K)             \
    switch (K) {                        \
        ENUM_CASE(Token::CONST);        \
        ENUM_CASE(Token::DEF);          \
        ENUM_CASE(Token::MAIN);         \
        ENUM_CASE(Token::RETURN);       \
        BAD_ENUM_CASE(Token::KW_COUNT); \
    }

#define FOEREACH_TYPE(T)                  \
    switch (T) {                          \
        ENUM_CASE(Token::INT);            \
        BAD_ENUM_CASE(Token::TYPE_COUNT); \
    }


    static const char *opToStr(Token::Operator b) {
        FOEREACH_OP(b);
        crash("Unsuppirted operator {}", b);
    }
    static const char *kwToStr(Token::Keyword k) {
        FOEREACH_KEYWORD(k);
        crash("Unsuppirted keyword {}", k);
    };

    static const char *typeToStr(Token::BuiltinType t) {
        FOEREACH_TYPE(t);
        crash("Unsuppirted keyword {}", t);
    };
};

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

template<>
struct fmt::formatter<Token> : formatter<std::string> {
    template<typename FormatContext>
    auto format(Token t, FormatContext &ctx) {
        std::string name = "Invalid token";
        // clang-format off
        switch (t.type()) {
            bcase TokenType::T_OP:
                name = fmt::format("{}({})", t.type(), Token::opToStr(t.get<Token::Operator>()));
            bcase TokenType::T_EOF:
                name = fmt::format("{}()",   t.type());
            bcase TokenType::T_IDENTIFIER:
                name = fmt::format("{}({})", t.type(), t.get<Token::IdentifierHashT>());
            bcase TokenType::T_KEYWORD:
                name = fmt::format("{}({})", t.type(), Token::kwToStr(t.get<Token::Keyword>()));
            bcase TokenType::T_NUM:
                name = fmt::format("{}({})", t.type(), t.get<Int64>());
            bcase TokenType::T_BUILTIN_TYPE:
                name = fmt::format("{}({})", t.type(), Token::typeToStr(t.get<Token::BuiltinType>()));
            bcase TokenType::T_STR:
                todo();
        }
        // clang-format on


        return formatter<string_view>::format(name, ctx);
    }
};

#endif  // TOKEN_HPP
