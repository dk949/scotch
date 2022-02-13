#ifndef BUILTINS_HPP
#define BUILTINS_HPP
#include "token.hpp"
#include "types.hpp"


namespace Lex {

struct Builtins {
private:
    static bool m_i;
public:

    const static Map<StringView, Token::BuiltinType> typeMap;
    const static Map<StringView, Token::Keyword> kwMap;
    const static Map<StringView, Token::Operator> opMap;

    Builtins();

    static bool isopstart(char c);
};

}

#endif  // BUILTINS_HPP
