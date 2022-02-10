#include "lexer.hpp"

Lexer::Lexer(String input)
        : m_input(std::move(input))
        , m_current(m_input.begin()) {
    trace();
    verify(!m_input.empty());
}

Vector<Token> Lexer::parseAll() {
    trace();
    Vector<Token> out;
    do {
        const auto tok = parseToken();

        out.push_back(tok);
    } while (!out.back().isEOF());

    return out;
}

Token Lexer::parseToken() {
    trace();
    while (skipComment() || skipSpace()) { }

    if (std::isalpha(*m_current)) {
        return parseWord();
    }
    if (std::isdigit(*m_current)) {
        return parseNumber();
    }

    if (Builtins::isopstart(*m_current)) {
        return parseOperator();
    }

    if (isEOF()) {
        return Token {};
    }

    crash("Unrecognised token on character {} (hex: {:#x})", *m_current, *m_current);
}


bool Lexer::skipSpace() {
    trace();
    bool found = false;
    while (!isEOF() && std::isspace(*m_current)) {
        found = true;
        m_current = std::next(m_current);
    }
    return found;
}

bool Lexer::skipComment() {
    trace();
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
    trace();
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
    trace();
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
        crash("floating point numbers are not suppoterd, expected number, got {}", *m_current);
    }

    return Token {Int64 {std::stoi(number)}};
}


Token Lexer::parseOperator() {
    trace();
    const StringView op {&*m_current, 1};
    // FIXME: doesn't handle multi=character operators like `==`
    if (Builtins::opMap.contains(op)) {
        m_current = std::next(m_current);
        return Token {Builtins::opMap.at(op)};
    }
    crash("Unknown operator {} (hex: {:#x})", op, *m_current);
}

bool Lexer::isEOF(Iter i) {
    trace();
    // avoids short circuiting
    return !(i != m_input.end() && *i != '\0');
}

bool Lexer::isEOF() {
    trace();
    return isEOF(m_current);
}

bool Lexer::isEOL() {
    trace();
    return *m_current == '\n' || (*m_current == '\r' && *(m_current = std::next(m_current)) == '\n');
}
