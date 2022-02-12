#ifndef TOKEN_HPP
#define TOKEN_HPP
#include "hash.hpp"
#include "log.hpp"
#include "token_type.hpp"
#include "types.hpp"

#include <compare>

struct Token {
public:
    using IdentifierHashT = uint64_t;
    using Order = std::partial_ordering;
    // using IdentifierPtr = char *;

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
        LBRACKET,
        RBRACKET,

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
        char *id;
    };

private:
    TokenType m_type;

    Value m_val;

public:

    explicit Token(Int64 val);
    explicit Token(Keyword val);
    explicit Token(Operator val);
    explicit Token(const char *val);
    explicit Token(BuiltinType val);
    explicit Token();


    Token(const Token &);
    Token &operator=(const Token &);

    Token(Token &&);
    Token &operator=(Token &&);


    ~Token();

    TokenType type() const;

    template<typename T>
    T get() const {
        static_assert(sizeof(T) < 0, "get used with an invalid type");
    }

    Order operator<=>(Int64 i) const;
    Order operator<=>(Keyword k) const;
    Order operator<=>(Operator o) const;
    Order operator<=>(StringView id) const;
    Order operator<=>(BuiltinType t) const;
    Order operator<=>(const Token &t) const;

    bool operator==(Int64 i) const;
    bool operator==(Keyword k) const;
    bool operator==(Operator o) const;
    bool operator==(StringView id) const;
    bool operator==(BuiltinType t) const;
    bool operator==(const Token &t) const;
    bool isEOF() const;
    bool isBinExpr() const;

    constexpr static IdentifierHashT hash(std::string_view sv) {
        return Tools::fnv_1a(sv);
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
template<> const char * Token::get<const char *>() const;
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
                name = fmt::format("{}({})", t.type(), t.get<const char *>());
            bcase TokenType::T_KEYWORD:
                name = fmt::format("{}({})", t.type(), Token::kwToStr(t.get<Token::Keyword>()));
            bcase TokenType::T_INT:
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
