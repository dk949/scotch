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
    struct {
        bool i32;
        bool f32;
    } flags;
    m_current = std::next(m_current);
    while (!isEOF() && std::isdigit(*m_current)) {
        number.push_back(*m_current);
        m_current = std::next(m_current);
    }
    if (std::isalpha(*m_current)) {
        switch (*m_current) {
            case 'i':
                flags.i32 = true;
                spdlog::debug("encountered an i32 literal");
                break;
            case 'f':
                flags.f32 = true;
                break;
            default:
                crash("Unknonw suffix. Expected f or i, got {}", *m_current);
        }
        m_current = std::next(m_current);
    }
    if (*m_current == '.') {
        crash("floating point numbers are not supported, expected number, got {}", *m_current);
    }

    if (flags.i32) {
        spdlog::debug("returning 32 bit int");
        return Lex::Token {Int32 {std::stoi(number)}};
    } else {
        spdlog::debug("returning 64 bit int");
        return Lex::Token {Int64 {std::stoll(number)}};
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
