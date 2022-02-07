#ifndef LEXER_HPP
#define LEXER_HPP
#include "builtins.hpp"
#include "token.hpp"
#include "types.hpp"

#include <charconv>

class Lexer {
private:
    using Iter = String::iterator;
    String m_input;
    Iter m_current;
    Iter eof;

public:
    NO_MOVE_OR_COPY(Lexer);

    explicit Lexer(String input);

    Vector<Token> parseAll();

private:
    Token parseToken();
    void skipSpace();
    void skipComment();
    Token parseWord();
    Token parseNumber();
    Token parseOperator();

    bool isEOF(Iter i);
    bool isEOF();
    bool isEOL();
};

#endif  // LEXER_HPP
