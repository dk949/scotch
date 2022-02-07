#ifndef PARSER_HPP
#define PARSER_HPP
#include "ast.hpp"
#include "tokenize.hpp"
#include "types.hpp"

class Parser {
    using Iter = Vector<Token>::iterator;
    Vector<Token> m_tokens;
    Iter m_current;
public:
    explicit Parser(Vector<Token> tokens);

    Ast::NodePtr makeNode();
    Ast::FunctionDeclPtr makeFunction();

    Ast::ValueType makeTypeAnnotation();

    bool isLast();

    bool isLast(Iter i);

    bool isReturn();

    bool isScopeBegin();

    bool isScopeEnd();
};

#endif  // PARSER_HPP
