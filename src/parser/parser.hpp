#ifndef PARSER_HPP
#define PARSER_HPP
#include "ast.hpp"
#include "lex.hpp"
#include "types.hpp"

namespace Parse {

class Parser {
    using Iter = Vector<Lex::Token>::iterator;
    Vector<Lex::Token> m_tokens;
    Iter m_current;
public:
    explicit Parser(Vector<Lex::Token> tokens);

    Ast::BlockPtr makeBlock();
    Ast::ExpressionPtr makeExpr();
    Ast::FunctionDeclPtr makeFunction();
    Ast::LiteralPtr makeLiteral();
    Ast::NodePtr makeNode();
    Ast::ProgramPtr makeProgram();
    Ast::ReturnPtr makeReturn();

    Ast::ValueType makeTypeAnnotation();
    Vector<Ast::ValueType> makeArgs();

    bool isLast();

    bool isLast(Iter i);

    bool isReturn();

    bool isScopeBegin();

    bool isScopeEnd();

    [[nodiscard]] Iter next() const;
};

}

#endif  // PARSER_HPP
