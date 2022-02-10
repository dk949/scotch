#include "parser.hpp"

Parser::Parser(Vector<Token> tokens)
        : m_tokens(std::move(tokens))
        , m_current(m_tokens.begin()) {
    trace();
}


Ast::NodePtr Parser::makeNode() {
    trace();
    if (*m_current == Token::DEF) {
        return makeFunction();
    }
    crash("cannot parse token {}", *m_current);
    todo();
}

Ast::FunctionDeclPtr Parser::makeFunction() {
    trace();
    m_current = next();
    verify_msg(!isLast(), "Expected function declaration after {} in function declaration. Found nothing", *std::prev(m_current));
    verify_msg(m_current->type() == TokenType::T_IDENTIFIER,  //
        "Expected function name after {} in function declaration. Found {}",
        *std::prev(m_current),
        *m_current);
    const StringView id = m_current->get<const char *>();
    m_current = next();
    const auto args = makeArgs();

    m_current = next();
    Ast::ValueType ret = makeTypeAnnotation();
    m_current = next();
    Ast::ScopePtr body = makeBlock();
    auto func = MakePtr<Ast::FunctionDecl>(String {id}, args, ret, body);

    return func;
}


Ast::BlockPtr Parser::makeBlock() {
    trace();
    verify_msg(*m_current == Token::LCURLY, "Expected left curly brace at the start of a scope, found {}", *m_current);
    auto body = MakePtr<Ast::Block>();
    while (true) {
        m_current = next();
        verify_msg(!isLast(), "Expected expression after {} in function body. Found nothing", *std::prev(m_current));
        if (*m_current == Token::RCURLY) {
            body->append(makeBlock());
        }
        if (*m_current == Token::LCURLY) {
            break;
        }
        body->append(makeNode());
    }
    return body;
}

Vector<Ast::ValueType> Parser::makeArgs() {
    trace();
    // FIXME: functions do not take arguments yet
    verify_msg(*m_current == Token::LBRACKET,
        "Expected right bracket after {} in function argument declaration. Found {}",
        *std::prev(m_current),
        *m_current);

    m_current = next();
    verify_msg(*m_current == Token::RBRACKET,
        "Expected left bracket after {} in function argument declaration. Found {}",
        *std::prev(m_current),
        *m_current);
    return {};
}

Ast::ValueType Parser::makeTypeAnnotation() {
    trace();
    verify_msg(!isLast(), "Expected type annotation after {}. Found nothing", *std::prev(m_current));

    if (*m_current == Token::COLON) {
        m_current = next();
        verify_msg(!isLast(), "Expected a type after {} in type annotation declaration, got nothing", *std::prev(m_current));

        // Handling builtin types
        if (m_current->type() == TokenType::T_BUILTIN_TYPE) {
            switch (m_current->get<Token::BuiltinType>()) {
                bcase Token::INT : {
                    m_current = next();
                    return Ast::ValueType::INT;
                }
                BAD_ENUM_CASE(Token::TYPE_COUNT);
            }
        }
        // TODO: handle user defined types
        todo();
    }
    // TODO: handle implicit types
    todo();
}


bool Parser::isLast() {
    trace();
    return isLast(m_current);
}

bool Parser::isLast(Iter i) {
    trace();
    return i == m_tokens.end();
}

bool Parser::isReturn() {
    trace();
    return m_current->type() == TokenType::T_KEYWORD && m_current->get<Token::Keyword>() == Token::RETURN;
}

bool Parser::isScopeBegin() {
    trace();
    if (*m_current == Token::LCURLY) {
        m_current = next();
        return true;
    }
    return false;
}

bool Parser::isScopeEnd() {
    trace();
    if (*m_current == Token::RCURLY) {
        m_current = next();
        return true;
    }
    return false;
}

Parser::Iter Parser::next() const {
    trace();
    debug("next token is {}", *std::next(m_current));
    return std::next(m_current);
}
