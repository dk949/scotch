#ifndef LEXER_HPP
#define LEXER_HPP
#include "class_tools.hpp"
#include "token.hpp"
#include "types.hpp"

#include <charconv>

namespace Lex {

class Lexer {
private:
    using Iter = String::iterator;
    String m_input;
    Iter m_current;
    Iter eof;

public:
    NO_MOVE_OR_COPY(Lexer);

    explicit Lexer(String input);
    ~Lexer() = default;

    Vector<Token> parseAll();

private:
    Token parseToken();
    bool skipSpace();
    bool skipComment();
    Token parseWord();
    Token parseNumber();
    Token parseOperator();

    bool isEOF(Iter i);
    bool isEOF();
    bool isEOL();
};

}

#endif  // LEXER_HPP
