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
    Parser(Vector<Token> tokens)
            : m_tokens(std::move(tokens))
            , m_current(m_tokens.begin()) { }

    Ast::NodePtr makeNode() {
        if (m_current->type() == TokenType::T_KEYWORD && m_current->get<Token::Keyword>() == Token::DEF) {
            return makeFunction();
        }
        todo();
    }

    Ast::FunctionDeclPtr makeFunction() {
        m_current = std::next(m_current);

        if (m_current->type() == TokenType::T_KEYWORD && m_current->get<Token::Keyword>() == Token::MAIN) {
            m_current = std::next(m_current);


            // Ignored, as long as it exists on entry point
            // makes you wander why we need it on the enty point
            makeTypeAnnotation();

            verify(isScopeBegin());

            auto block = MakePtr<Ast::Block>();

            m_current = std::next(m_current);
            while (!isScopeEnd()) {
                verify(!isLast());
                block->append(makeNode());
                m_current = std::next(m_current);
            }
            auto func = MakePtr<Ast::Entry>(block);
        }
        todo();
    }

    Ast::ValueType makeTypeAnnotation() {
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

    bool isLast() {
        return isLast(m_current);
    }

    bool isLast(Iter i) {
        return i != m_tokens.end();
    }

    bool isReturn() {
        return m_current->type() == TokenType::T_KEYWORD && m_current->get<Token::Keyword>() == Token::RETURN;
    }

    bool isScopeBegin() {
        return m_current->type() == TokenType::T_OP && m_current->get<Token::Operator>() == Token::LCURLY;
    }

    bool isScopeEnd() {
        return m_current->type() == TokenType::T_OP && m_current->get<Token::Operator>() == Token::RCURLY;
    }
};

#endif  // PARSER_HPP
