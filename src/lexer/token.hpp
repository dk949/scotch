#ifndef TOKEN_HPP
#define TOKEN_HPP
#include "hash.hpp"
#include "token_type.hpp"
#include "tools.hpp"
#include "types.hpp"

struct Token {
public:
    using IdentifierHashT = uint64_t;
    using IdentifierPtr   = const char *;

    enum class Keyword {
        CONST,
        DEF,
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
        IdentifierPtr id;
    };

private:
    TokenType m_type;

    Value m_val;

public:

    explicit Token(Int64 val);
    explicit Token(Keyword val);
    explicit Token(Operator val);
    explicit Token(IdentifierPtr val);
    explicit Token(BuiltinType val);
    explicit Token();


    Token(const Token &);
    Token &operator=(const Token &);

    Token(Token &&);
    Token &operator=(Token &&);


    ~Token();

    bool isEOF() const;
    TokenType type() const;

    template<typename T>
    T get() const {
        static_assert(sizeof(T) < 0, "get used with an invalid type");
    }

    constexpr static IdentifierHashT hash(std::string_view sv) {
        return CTHash::fnv_1a(sv);
    }

    static const char *opToStr(Token::Operator b);
    static const char *kwToStr(Token::Keyword k);
    static const char *typeToStr(Token::BuiltinType t);
};

// clang-format off
template<> Token::Operator Token::get<Token::Operator>() const;
template<> Int64 Token::get<Int64>() const;
template<> Token::Keyword Token::get<Token::Keyword>() const;
template<> Token::Operator Token::get<Token::Operator>() const;
template<> Token::IdentifierPtr Token::get<Token::IdentifierPtr>() const;
template<> Token::BuiltinType Token::get<Token::BuiltinType>() const;
template<> void Token::get<void>() const;
// clang-format on

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
                name = fmt::format("{}({})", t.type(), t.get<Token::IdentifierPtr>());
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
