#ifndef BUILTINS_HPP
#define BUILTINS_HPP
#include "token.hpp"
#include "types.hpp"


struct Builtins {
private:
    static bool m_i;
public:

    const static Map<StringView, Token::BuiltinType> typeMap;
    const static Map<StringView, Token::Keyword> kwMap;
    const static Map<StringView, Token::Operator> opMap;

    Builtins() {
        trace();
        if (m_i) {
            crash("{}",
                "Builtins needs to be instantiated exactly once,"
                " before entering main");
        }
        verify(kwMap.size() == to<size_t>(Token::KW_COUNT));
        verify(opMap.size() == to<size_t>(Token::OP_COUNT));
        verify(typeMap.size() == to<size_t>(Token::TYPE_COUNT));
        m_i = true;
    }

    static bool isopstart(char c);
};

#endif  // BUILTINS_HPP
