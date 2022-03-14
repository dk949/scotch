#include "lexer.hpp"

#include "builtins.hpp"
#include "common.hpp"
#include "ftrace.hpp"
#include "log.hpp"

Lex::Lexer::Lexer(String input)
        : m_input(std::move(input))
        , m_current(m_input.begin()) {
    ftrace();
    verify(!m_input.empty());
}

Vector<Lex::Token> Lex::Lexer::parseAll() {
    ftrace();
    Vector<Lex::Token> out;
    do {
        const auto tok = parseToken();

        out.push_back(tok);
    } while (!out.back().isEOF());

    return out;
}

Lex::Token Lex::Lexer::parseToken() {
    ftrace();
    while (skipComment() || skipSpace()) { }

    if (isEOF()) {
        return Lex::Token {};
    }

    if (std::isalpha(*m_current)) {
        return parseWord();
    }
    if (std::isdigit(*m_current)) {
        return parseNumber();
    }

    if (Builtins::isopstart(*m_current)) {
        return parseOperator();
    }


    crash("Unrecognised token on character {} (hex: {:#x})", *m_current, *m_current);
}


bool Lex::Lexer::skipSpace() {
    ftrace();
    bool found = false;
    while (!isEOF() && std::isspace(*m_current)) {
        found = true;
        m_current = std::next(m_current);
    }
    return found;
}

bool Lex::Lexer::skipComment() {
    ftrace();
    bool found = false;
    if (!isEOF(m_current)                            //
        && *m_current == '/'                         //
        && !isEOF(m_current = std::next(m_current))  //
        && *m_current == '/') {
        m_current = std::next(m_current);
        while (!isEOF() && !isEOL()) {
            found = true;
            m_current = std::next(m_current);
        }
    }
    return found;
}


Lex::Token Lex::Lexer::parseWord() {
    ftrace();
    String word {*m_current};
    m_current = std::next(m_current);
    while (!isEOF() && std::isalnum(*m_current)) {
        word.push_back(*m_current);
        m_current = std::next(m_current);
    }

    if (Builtins::kwMap.contains(word)) {
        return Lex::Token {Builtins::kwMap.at(word)};
    }

    if (Builtins::typeMap.contains(word)) {
        return Lex::Token {Builtins::typeMap.at(word)};
    }

    return Lex::Token {to<const char *>(word.c_str())};
}

Lex::Token Lex::Lexer::parseNumber() {
    ftrace();
    String number {*m_current};
    auto numType = Lex::Token::I64;
    m_current = std::next(m_current);

    while (!isEOF() && (std::isdigit(*m_current) || *m_current == '.')) {
        if (*m_current == '.')
            numType = Lex::Token::F64;
        number.push_back(*m_current);
        m_current = std::next(m_current);
    }

    if (std::isalpha(*m_current)) {
        numType = checkSuffix(*m_current);
        m_current = std::next(m_current);
    }

    return numTypeToToken(numType, number);
}

Lex::Token::BuiltinType Lex::Lexer::checkSuffix(char ch) {
    switch (ch) {
        case 'i':
            return Lex::Token::I32;
        case 'l':
            return Lex::Token::I64;
        case 'f':
            return Lex::Token::F32;
        case 'd':
            return Lex::Token::F64;
        default:
            crash("Unknonw suffix. Expected f or i, got {}", ch);
    }
}

Lex::Token Lex::Lexer::numTypeToToken(Lex::Token::BuiltinType numType, const String &number) {
    ftrace();
    switch (numType) {
        case Lex::Token::I32:
            return Lex::Token {Int32 {std::stoi(number)}};
        case Lex::Token::I64:
            return Lex::Token {Int64 {std::stoll(number)}};
        case Lex::Token::F32:
            return Lex::Token {Float32 {std::stof(number)}};
        case Lex::Token::F64:
            return Lex::Token {Float64 {std::stod(number)}};
        default:
            unreachable("Should not be possible for this to be any other token");
    }
}

Lex::Token Lex::Lexer::parseOperator() {
    ftrace();
    const StringView op {&*m_current, 1};
    fixme("operator parser doesn't handle multi-character operators like `==`");
    if (Builtins::opMap.contains(op)) {
        m_current = std::next(m_current);
        return Lex::Token {Builtins::opMap.at(op)};
    }
    crash("Unknown operator {} (hex: {:#x})", op, *m_current);
}

bool Lex::Lexer::isEOF(Iter i) {
    ftrace();
    // avoids short-circuiting
    if (i == m_input.end()) {
        return true;
    }
    if (*i == '\0') {
        return true;
    }
    return false;
}

bool Lex::Lexer::isEOF() {
    ftrace();
    return isEOF(m_current);
}

bool Lex::Lexer::isEOL() {
    ftrace();
    return *m_current == '\n' || (*m_current == '\r' && *(m_current = std::next(m_current)) == '\n');
}
