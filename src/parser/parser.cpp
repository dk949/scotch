#include "parser.hpp"

Parser::Parser(Vector<Token> tokens)
        : m_tokens(std::move(tokens))
        , m_current(m_tokens.begin()) { }


Ast::NodePtr Parser::makeNode() {
    if (m_current->type() == TokenType::T_KEYWORD && m_current->get<Token::Keyword>() == Token::DEF) {
        return makeFunction();
    }
    todo();
}

Ast::FunctionDeclPtr Parser::makeFunction() {
    m_current = std::next(m_current);

    todo();
}

Ast::ValueType Parser::makeTypeAnnotation() {
    if (!isLast() && m_current->type() == TokenType::T_OP && m_current->get<Token::Operator>() == Token::COLON) {
        m_current = std::next(m_current);
        if (!isLast() && m_current->type() == TokenType::T_BUILTIN_TYPE) {
            switch (m_current->get<Token::BuiltinType>()) {
                bcase Token::INT : {
                    m_current = std::next(m_current);
                    return Ast::ValueType::INT;
                }
                BAD_ENUM_CASE(Token::TYPE_COUNT);
            }
        }
        // handle user defined types
        todo();
    }
    // handle implicit types
    todo();
}


bool Parser::isLast() {
    return isLast(m_current);
}

bool Parser::isLast(Iter i) {
    return i != m_tokens.end();
}

bool Parser::isReturn() {
    return m_current->type() == TokenType::T_KEYWORD && m_current->get<Token::Keyword>() == Token::RETURN;
}

bool Parser::isScopeBegin() {
    return m_current->type() == TokenType::T_OP && m_current->get<Token::Operator>() == Token::LCURLY;
}

bool Parser::isScopeEnd() {
    return m_current->type() == TokenType::T_OP && m_current->get<Token::Operator>() == Token::RCURLY;
}
