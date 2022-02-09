#include "lexer.hpp"

Lexer::Lexer(String input)
        : m_input(std::move(input))
        , m_current(m_input.begin()) {
    verify(!m_input.empty());
}

Vector<Token> Lexer::parseAll() {
    Vector<Token> out;
    do {
        const auto tok = parseToken();

        out.push_back(tok);
    } while (!out.back().isEOF());

    return out;
}

Token Lexer::parseToken() {
    skipSpace();
    skipComment();
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
    crash("Unrecognised token on character {}", *m_current);
}


void Lexer::skipSpace() {
    while (!isEOF() && std::isspace(*m_current)) {
        m_current = std::next(m_current);
    }
}

void Lexer::skipComment() {
    if (!isEOF(m_current)                            //
        && *m_current == '/'                         //
        && !isEOF(m_current = std::next(m_current))  //
        && *m_current == '/') {
        m_current = std::next(m_current);
        while (!isEOF() && !isEOL()) {
            m_current = std::next(m_current);
        }
    }
}


Token Lexer::parseWord() {
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
    // m_current can be converted to char* by dereferencing and taking it's address.
    String op = {*m_current};

    m_current = std::next(m_current);
    while (!isEOF() && Builtins::isopstart(*m_current)) {
        op.push_back(*m_current);
        m_current = std::next(m_current);
    }


    if (Builtins::opMap.contains(op)) {
        return Token {Builtins::opMap.at(op)};
    }
    crash("Unknown operator {} (hex: {:#x})", op, *m_current);
}

bool Lexer::isEOF(Iter i) {
    // avoids short circuiting
    return !(i != m_input.end() && *i != '\0');
}

bool Lexer::isEOF() {
    return isEOF(m_current);
}

bool Lexer::isEOL() {
    return *m_current == '\n' || (*m_current == '\r' && *(m_current = std::next(m_current)) == '\n');
}
