#ifndef TOKEN_TYPE_HPP
#define TOKEN_TYPE_HPP

#include "switch_tools.hpp"
#include "common.hpp"

#include <fmt/format.h>

namespace Lex {

enum class TokenType {
    T_KEYWORD,
    T_IDENTIFIER,

    T_I32,
    T_I64,

    T_F32,
    T_F64,

    T_STR,

    T_OP,

    T_BUILTIN_TYPE,

    T_EOF,
};

}

#undef ENUM_DO
#define ENUM_DO(TOK) name = Tools::shortEnumName(#TOK);

template<>
struct fmt::formatter<Lex::TokenType> : formatter<std::string_view> {
public:
    template<typename FormatContext>
    auto format(Lex::TokenType t, FormatContext &ctx) {
        std::string name = "Invalid token";
        switch (t) {
            ENUM_CASE(Lex::TokenType::T_KEYWORD);
            ENUM_CASE(Lex::TokenType::T_IDENTIFIER);

            ENUM_CASE(Lex::TokenType::T_I32);
            ENUM_CASE(Lex::TokenType::T_I64);

            ENUM_CASE(Lex::TokenType::T_F32);
            ENUM_CASE(Lex::TokenType::T_F64);

            ENUM_CASE(Lex::TokenType::T_STR);

            ENUM_CASE(Lex::TokenType::T_OP);

            ENUM_CASE(Lex::TokenType::T_BUILTIN_TYPE);

            ENUM_CASE(Lex::TokenType::T_EOF);
        }
        return formatter<string_view>::format(name, ctx);
    }
};



#endif  // TOKEN_TYPE_HPP
