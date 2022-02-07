#ifndef TOKEN_TYPE_HPP
#define TOKEN_TYPE_HPP

#include "tools.hpp"

#include <iostream>
#include <spdlog/spdlog.h>

enum class TokenType {
    T_KEYWORD,
    T_IDENTIFIER,

    T_NUM,
    T_STR,

    T_OP,

    T_BUILTIN_TYPE,

    T_EOF,
};

#define FOREACH_TOKEN(TOK)                    \
    switch (TOK) {                            \
        ENUM_CASE(TokenType::T_KEYWORD);      \
        ENUM_CASE(TokenType::T_IDENTIFIER);   \
                                              \
        ENUM_CASE(TokenType::T_NUM);          \
        ENUM_CASE(TokenType::T_STR);          \
                                              \
        ENUM_CASE(TokenType::T_OP);           \
                                              \
        ENUM_CASE(TokenType::T_BUILTIN_TYPE); \
                                              \
        ENUM_CASE(TokenType::T_EOF);          \
    }


#undef ENUM_DO
#define ENUM_DO(TOK) name = #TOK;



#endif  // TOKEN_TYPE_HPP
