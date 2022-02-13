#include "lexer.hpp"

#include "builtins.hpp"
#include "common.hpp"
#include "ftrace.hpp"
#include "log.hpp"

Lexer::Lexer(String input)
        : m_input(std::move(input))
        , m_current(m_input.begin()) {
    ftrace();
    verify(!m_input.empty());
}

Vector<Token> Lexer::parseAll() {
    ftrace();
    Vector<Token> out;
    do {
        const auto tok = parseToken();

        out.push_back(tok);
    } while (!out.back().isEOF());

    return out;
}

Token Lexer::parseToken() {
    ftrace();
    while (skipComment() || skipSpace()) { }

    if (isEOF()) {
        return Token {};
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


bool Lexer::skipSpace() {
    ftrace();
    bool found = false;
    while (!isEOF() && std::isspace(*m_current)) {
        found = true;
        m_current = std::next(m_current);
    }
    return found;
}

bool Lexer::skipComment() {
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


Token Lexer::parseWord() {
    ftrace();
    String word {*m_current};
    m_current = std::next(m_current);
    while (!isEOF() && std::isalnum(*m_current)) {
        word.push_back(*m_current);
        m_current = std::next(m_current);
    }

    if (Builtins::kwMap.contains(word)) {
        return Token {Builtins::kwMap.at(word)};
    }

    if (Builtins::typeMap.contains(word)) {
        return Token {Builtins::typeMap.at(word)};
    }

    return Token {to<const char *>(word.c_str())};
}

Token Lexer::parseNumber() {
    ftrace();
    String number {*m_current};
    m_current = std::next(m_current);
    while (!isEOF() && std::isdigit(*m_current)) {
        number.push_back(*m_current);
        m_current = std::next(m_current);
    }
    if (std::isalpha(*m_current)) {
        crash("integer suffixes are not supported, expected number, got {}", *m_current);
    }
    if (*m_current == '.') {
        crash("floating point numbers are not supported, expected number, got {}", *m_current);
    }

    return Token {Int64 {std::stoi(number)}};
}


Token Lexer::parseOperator() {
    ftrace();
    const StringView op {&*m_current, 1};
    fixme("{}", "operator parser doesn't handle multi-character operators like `==`");
    if (Builtins::opMap.contains(op)) {
        m_current = std::next(m_current);
        return Token {Builtins::opMap.at(op)};
    }
    crash("Unknown operator {} (hex: {:#x})", op, *m_current);
}

bool Lexer::isEOF(Iter i) {
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

bool Lexer::isEOF() {
    ftrace();
    return isEOF(m_current);
}

bool Lexer::isEOL() {
    ftrace();
    return *m_current == '\n' || (*m_current == '\r' && *(m_current = std::next(m_current)) == '\n');
}
