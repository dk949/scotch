#ifndef TOKEN_HPP
#define TOKEN_HPP
#include "ftrace.hpp"
#include "gsl/gsl"
#include "hash.hpp"
#include "log.hpp"
#include "token_type.hpp"
#include "types.hpp"

#include <compare>

namespace Lex {

struct Token {
public:
    using IdentifierHashT = uint64_t;
    using Order = std::partial_ordering;
    // using IdentifierPtr = char *;

    enum class Keyword {
        CONST,
        DEF,
        MODULE,
        RETURN,

        KW_COUNT,
    };
    enum class Operator {
        PLUS,
        MINUS,
        EQ,
        COLON,
        SEMICOLON,
        LCURLY,
        RCURLY,
        LBRACKET,
        RBRACKET,

        OP_COUNT,
    };

    enum class BuiltinType {
        I32,
        I64,

        F32,
        F64,

        TYPE_COUNT,
    };
    using enum Keyword;
    using enum Operator;
    using enum BuiltinType;

    union Value {
        Int32 i32;
        Int64 i64;

        Float32 f32;
        Float64 f64;

        Keyword kw;
        Operator op;
        BuiltinType type;
        gsl::owner<char *> id;
    };

private:
    TokenType m_type;

    Value m_val;

public:

    explicit Token(Int32 val);
    explicit Token(Int64 val);

    explicit Token(Float32 val);
    explicit Token(Float64 val);

    explicit Token(Keyword val);
    explicit Token(Operator val);
    explicit Token(const char *val);
    explicit Token(BuiltinType val);
    explicit Token();


    Token(const Token &);
    Token &operator=(const Token &);

    Token(Token &&) noexcept;
    Token &operator=(Token &&) noexcept;


    ~Token();

    [[nodiscard]] TokenType type() const;

    template<typename T>
    T get() const {
        static_assert(Tools::always_false_v<T>, "get used with an invalid type");
    }

    Order operator<=>(Int32 i) const;
    Order operator<=>(Int64 i) const;
    Order operator<=>(Float32 i) const;
    Order operator<=>(Float64 i) const;
    Order operator<=>(Keyword k) const;
    Order operator<=>(Operator o) const;
    Order operator<=>(StringView id) const;
    Order operator<=>(BuiltinType t) const;
    Order operator<=>(const Token &t) const;

    bool operator==(Int32 i) const;
    bool operator==(Int64 i) const;
    bool operator==(Float32 i) const;
    bool operator==(Float64 i) const;
    bool operator==(Keyword k) const;
    bool operator==(Operator o) const;
    bool operator==(StringView id) const;
    bool operator==(BuiltinType t) const;
    bool operator==(const Token &t) const;
    [[nodiscard]] bool isEOF() const;
    [[nodiscard]] bool isBinExpr() const;

    constexpr static IdentifierHashT hash(std::string_view sv) {
        return Tools::fnv_1a(sv);
    }

    static StringView opToStr(Token::Operator b);
    static StringView kwToStr(Token::Keyword k);
    static StringView typeToStr(Token::BuiltinType t);
};

// clang-format off
template<> [[nodiscard]] Token::Operator Token::get<Token::Operator>() const;
template<> [[nodiscard]] Int32 Token::get<Int32>() const;
template<> [[nodiscard]] Int64 Token::get<Int64>() const;
template<> [[nodiscard]] Float32 Token::get<Float32>() const;
template<> [[nodiscard]] Float64 Token::get<Float64>() const;
template<> [[nodiscard]] Token::Keyword Token::get<Token::Keyword>() const;
template<> [[nodiscard]] const char * Token::get<const char *>() const;
template<> [[nodiscard]] Token::BuiltinType Token::get<Token::BuiltinType>() const;
template<> void Token::get<void>() const;
// clang-format on

}  // namespace Lex

template<>
struct fmt::formatter<Lex::Token> : formatter<std::string> {
    template<typename FormatContext>
    auto format(const Lex::Token &t, FormatContext &ctx) {
        ftrace();
        std::string name = "Invalid token";
        // clang-format off
        switch (t.type()) {
            bcase Lex::TokenType::T_OP:
                name = fmt::format("{}({})", t.type(), Lex::Token::opToStr(t.get<Lex::Token::Operator>()));
            bcase Lex::TokenType::T_EOF:
                name = fmt::format("{}()",   t.type());
            bcase Lex::TokenType::T_IDENTIFIER:
                name = fmt::format("{}({})", t.type(), t.get<const char *>());
            bcase Lex::TokenType::T_KEYWORD:
                name = fmt::format("{}({})", t.type(), Lex::Token::kwToStr(t.get<Lex::Token::Keyword>()));
            bcase Lex::TokenType::T_I32:
                name = fmt::format("{}({})", t.type(), t.get<Int32>());
            bcase Lex::TokenType::T_I64:
                name = fmt::format("{}({})", t.type(), t.get<Int64>());
            bcase Lex::TokenType::T_F32:
                name = fmt::format("{}({})", t.type(), t.get<Float32>());
            bcase Lex::TokenType::T_F64:
                name = fmt::format("{}({})", t.type(), t.get<Float64>());
            bcase Lex::TokenType::T_BUILTIN_TYPE:
                name = fmt::format("{}({})", t.type(), Lex::Token::typeToStr(t.get<Lex::Token::BuiltinType>()));
            bcase Lex::TokenType::T_STR:
                fixme("string literals are not being handled");
                todo();
        }
        // clang-format on


        return formatter<string_view>::format(name, ctx);
    }
};

#endif  // TOKEN_HPP
