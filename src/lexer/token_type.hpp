#ifndef TOKEN_TYPE_HPP
#define TOKEN_TYPE_HPP

#include "tools.hpp"

#include <spdlog/spdlog.h>

enum class TokenType {
    T_KEYWORD,
    T_IDENTIFIER,

    T_INT,
    T_STR,

    T_OP,

    T_BUILTIN_TYPE,

    T_EOF,
};


#undef ENUM_DO
#define ENUM_DO(TOK) name = #TOK;

template<>
struct fmt::formatter<TokenType> : formatter<std::string_view> {
    template<typename FormatContext>
    auto format(TokenType t, FormatContext &ctx) {
        std::string name = "Invalid token";
        switch (t) {
            ENUM_CASE(TokenType::T_KEYWORD);
            ENUM_CASE(TokenType::T_IDENTIFIER);

            ENUM_CASE(TokenType::T_INT);
            ENUM_CASE(TokenType::T_STR);

            ENUM_CASE(TokenType::T_OP);

            ENUM_CASE(TokenType::T_BUILTIN_TYPE);

            ENUM_CASE(TokenType::T_EOF);
        }
        return formatter<string_view>::format(name, ctx);
    }
};



#endif  // TOKEN_TYPE_HPP
