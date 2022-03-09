#include "parser.hpp"

#include "ftrace.hpp"
#include "log.hpp"

Parse::Parser::Parser(Vector<Lex::Token> tokens)
        : m_tokens(std::move(tokens))
        , m_current(m_tokens.begin()) {
    ftrace();
}


Ast::ProgramPtr Parse::Parser::makeProgram() {
    ftrace();
    auto program = MakePtr<Ast::Program>();
    while (!isLast()) {
        program->append(makeNode());
    }
    return program;
}

Ast::NodePtr Parse::Parser::makeNode() {
    ftrace();
    if (*m_current == Lex::Token::DEF) {
        return makeFunction();
    }
    if (*m_current == Lex::Token::RETURN) {
        return makeReturn();
    }
    crash("cannot parse token {}", *m_current);
}

Ast::ReturnPtr Parse::Parser::makeReturn() {
    ftrace();
    verify_msg(*m_current == Lex::Token::RETURN, "Expected return keyword in the return statement, got {}", *m_current);
    m_current = std::next(m_current);
    verify_msg(!isLast(),  //
        "Expected expression or semicolon after {} in return statement. Found nothing",
        *std::prev(m_current));
    auto ret = MakePtr<Ast::Return>(makeExpr());
    m_current = std::next(m_current);
    return ret;
}

Ast::ExpressionPtr Parse::Parser::makeExpr() {
    ftrace();
    switch (m_current->type()) {
        case Lex::TokenType::T_I32:
        case Lex::TokenType::T_I64:
        case Lex::TokenType::T_F32:
        case Lex::TokenType::T_F64:
        case Lex::TokenType::T_STR:
            return makeLiteral();
        case Lex::TokenType::T_KEYWORD:
            fixme("keywords are not handled in expressions");
            todo();
        case Lex::TokenType::T_IDENTIFIER:
            fixme("identifiers are not handled in expressions");
            todo();
        case Lex::TokenType::T_OP:
            if (m_current->isBinExpr()) {
                fixme("Not parsing binary expression {}", *m_current);
                todo();
            }
            if (*m_current == Lex::Token::SEMICOLON) {
                fixme("Reached semicolon in expression");
                todo();
            }
            fixme("Not handling any operator other then binary operators in expressions");
            todo();
        case Lex::TokenType::T_BUILTIN_TYPE:
            crash("unexpected BUILTIN_TYPE after {} in expression", *std::prev(m_current));
        case Lex::TokenType::T_EOF:
            crash("unexpected EOF after {} in expression", *std::prev(m_current));
    }
    unreachable("exhaustive enum handling");
}

Ast::LiteralPtr Parse::Parser::makeLiteral() {
    ftrace();
    switch (m_current->type()) {
        case Lex::TokenType::T_I32:
            return MakePtr<Ast::Literal>(Ast::Value {m_current->get<Int32>()});
        case Lex::TokenType::T_I64:
            return MakePtr<Ast::Literal>(Ast::Value {m_current->get<Int64>()});
        case Lex::TokenType::T_F32:
            return MakePtr<Ast::Literal>(Ast::Value {m_current->get<Float32>()});
        case Lex::TokenType::T_F64:
            return MakePtr<Ast::Literal>(Ast::Value {m_current->get<Float64>()});
        case Lex::TokenType::T_STR:
            fixme("string literals are not handled");
            todo();
        default:
            unreachable("Handleled all literal types. Unexpected {} in literal", m_current->type());
    }
}

Ast::FunctionDeclPtr Parse::Parser::makeFunction() {
    ftrace();
    m_current = next();
    verify_msg(!isLast(),  //
        "Expected function declaration after {} in function declaration. Found nothing",
        *std::prev(m_current));
    verify_msg(m_current->type() == Lex::TokenType::T_IDENTIFIER,  //
        "Expected function name after {} in function declaration. Found {}",
        *std::prev(m_current),
        *m_current);
    const StringView id = m_current->get<const char *>();
    m_current = next();
    const auto args = makeArgs();

    m_current = next();
    Ast::ValueType ret = makeTypeAnnotation();
    Ast::ScopePtr body = makeBlock();
    auto func = MakePtr<Ast::FunctionDecl>(String {id}, args, ret, body);

    return func;
}


Ast::BlockPtr Parse::Parser::makeBlock() {
    ftrace();
    verify_msg(*m_current == Lex::Token::LCURLY, "Expected left curly brace at the start of a scope, found {}", *m_current);
    auto body = MakePtr<Ast::Block>();
    while (true) {
        m_current = next();
        verify_msg(!isLast(),  //
            "Expected expression after {} in function body. Found nothing",
            *std::prev(m_current));
        if (*m_current == Lex::Token::RCURLY) {
            m_current = std::next(m_current);
            break;
        }
        if (*m_current == Lex::Token::LCURLY) {
            body->append(makeBlock());
        }
        body->append(makeNode());
    }
    return body;
}

Vector<Ast::ValueType> Parse::Parser::makeArgs() {
    ftrace();
    fixme("functions do not take any arguments yet");
    verify_msg(*m_current == Lex::Token::LBRACKET,
        "Expected right bracket after {} in function argument declaration. Found {}",
        *std::prev(m_current),
        *m_current);

    m_current = next();
    verify_msg(*m_current == Lex::Token::RBRACKET,
        "Expected left bracket after {} in function argument declaration. Found {}",
        *std::prev(m_current),
        *m_current);
    return {};
}

Ast::ValueType Parse::Parser::makeTypeAnnotation() {
    ftrace();
    verify_msg(!isLast(), "Expected type annotation after {}. Found nothing", *std::prev(m_current));

    if (*m_current == Lex::Token::COLON) {
        m_current = next();
        verify_msg(!isLast(), "Expected a type after {} in type annotation declaration, got nothing", *std::prev(m_current));

        // Handling builtin types
        if (m_current->type() == Lex::TokenType::T_BUILTIN_TYPE) {
            switch (m_current->get<Lex::Token::BuiltinType>()) {
                bcase Lex::Token::I32 : {
                    m_current = next();
                    return Ast::ValueType::I32;
                }
                bcase Lex::Token::I64 : {
                    m_current = next();
                    return Ast::ValueType::I64;
                }
                bcase Lex::Token::F32 : {
                    m_current = next();
                    return Ast::ValueType::F32;
                }
                bcase Lex::Token::F64 : {
                    m_current = next();
                    return Ast::ValueType::F64;
                }
                BAD_ENUM_CASE(Lex::Token::TYPE_COUNT);
            }
        }
        fixme("user defined types are not handled");
        todo();
    }
    fixme("implicit types are not handled");
    todo();
}


bool Parse::Parser::isLast() {
    ftrace();
    return isLast(m_current);
}

bool Parse::Parser::isLast(Iter i) {
    ftrace();
    verify(i != m_tokens.end());
    return i->type() == Lex::TokenType::T_EOF;
}

bool Parse::Parser::isReturn() {
    ftrace();
    return m_current->type() == Lex::TokenType::T_KEYWORD && m_current->get<Lex::Token::Keyword>() == Lex::Token::RETURN;
}

bool Parse::Parser::isScopeBegin() {
    ftrace();
    if (*m_current == Lex::Token::LCURLY) {
        m_current = next();
        return true;
    }
    return false;
}

bool Parse::Parser::isScopeEnd() {
    ftrace();
    if (*m_current == Lex::Token::RCURLY) {
        m_current = next();
        return true;
    }
    return false;
}

Parse::Parser::Iter Parse::Parser::next() const {
    ftrace();
    return std::next(m_current);
}
