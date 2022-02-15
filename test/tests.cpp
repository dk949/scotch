#include "file.hpp"

#include <catch2/catch.hpp>

TEST_CASE("state to mode conversion", "[Tools::File]") {
    using Catch::Matchers::Equals;
    SECTION("valid mode states") {
        REQUIRE_THAT(Tools::File::stateToMode(Tools::File::Read), Equals("r"));
        REQUIRE_THAT(Tools::File::stateToMode(Tools::File::Read | Tools::File::Binary), Equals("rb"));
        REQUIRE_THAT(Tools::File::stateToMode(Tools::File::Write), Equals("w"));
        REQUIRE_THAT(Tools::File::stateToMode(Tools::File::Write | Tools::File::Binary), Equals("wb"));
        REQUIRE_THAT(Tools::File::stateToMode(Tools::File::Append), Equals("a"));
        REQUIRE_THAT(Tools::File::stateToMode(Tools::File::Append | Tools::File::Binary), Equals("ab"));
        REQUIRE_THAT(Tools::File::stateToMode(Tools::File::Read | Tools::File::Write), Equals("w+"));
        REQUIRE_THAT(Tools::File::stateToMode(Tools::File::Read | Tools::File::Write | Tools::File::Binary), Equals("w+b"));
        REQUIRE_THAT(Tools::File::stateToMode(Tools::File::Read | Tools::File::Append), Equals("a+"));
        REQUIRE_THAT(Tools::File::stateToMode(Tools::File::Read | Tools::File::Append | Tools::File::Binary), Equals("a+b"));
    }

    SECTION("invalid mode states") {
        REQUIRE(Tools::File::stateToMode(Tools::File::Write | Tools::File::Append) == nullptr);
        REQUIRE(Tools::File::stateToMode(Tools::File::Write | Tools::File::Append | Tools::File::Binary) == nullptr);
        REQUIRE(Tools::File::stateToMode(Tools::File::Write | Tools::File::Read | Tools::File::Append) == nullptr);
        REQUIRE(Tools::File::stateToMode(Tools::File::Write | Tools::File::Read | Tools::File::Append | Tools::File::Binary) == nullptr);
        REQUIRE(Tools::File::stateToMode(Tools::File::Binary) == nullptr);
        REQUIRE(Tools::File::stateToMode(0u) == nullptr);
        REQUIRE(Tools::File::stateToMode(0b10000001) == nullptr);
        REQUIRE(Tools::File::stateToMode(0b01000001) == nullptr);
        REQUIRE(Tools::File::stateToMode(0b01100001) == nullptr);
        REQUIRE(Tools::File::stateToMode(0b00010001) == nullptr);
    }
}

TEST_CASE("StringView to char * conversion", "[Tools::File]") {
    using Catch::Matchers::Equals;
    SECTION("Simple cases") {
        StringView test1 = "test";
        REQUIRE_THAT(Tools::File::svToCharPtr(svalloca(test1), test1), Equals("test"));

        StringView test2 = "";
        REQUIRE_THAT(Tools::File::svToCharPtr(svalloca(test2), test2), Equals(""));

        StringView test3 = "\0\0\0";
        REQUIRE_THAT(Tools::File::svToCharPtr(svalloca(test3), test3), Equals(""));
    }

    SECTION("Complex cases") {
        String mainString = "hello world";
        StringView test1 {mainString.begin(), mainString.begin() + 5};
        REQUIRE_THAT(Tools::File::svToCharPtr(svalloca(test1), test1), Equals("hello"));

        StringView test2 {mainString.begin() + 6, mainString.end()};
        REQUIRE_THAT(Tools::File::svToCharPtr(svalloca(test2), test2), Equals("world"));

        StringView test3 {mainString.begin() + 5, mainString.begin() + 6};
        REQUIRE_THAT(Tools::File::svToCharPtr(svalloca(test3), test3), Equals(" "));

        StringView test4 {mainString.begin(), mainString.begin()};
        REQUIRE_THAT(Tools::File::svToCharPtr(svalloca(test4), test4), Equals(""));
    }
}

#include "comp.hpp"
#include "lex.hpp"
#include "log.hpp"
#include "parse.hpp"

TEST_CASE("lexing  shortterm_goal.scot", "[Lex::Lexer]") {
    using Catch::Matchers::Equals;
    const auto inputProgram = Tools::loadFile("./test/shortterm_goal.scot");

    Lex::Lexer lex {inputProgram};
    const auto tokens = lex.parseAll();
    const auto expectedTokens = Vector<Lex::Token> {
        Lex::Token {Lex::Token::DEF},
        Lex::Token {"main"},
        Lex::Token {Lex::Token::LBRACKET},
        Lex::Token {Lex::Token::RBRACKET},
        Lex::Token {Lex::Token::COLON},
        Lex::Token {Lex::Token::INT},
        Lex::Token {Lex::Token::LCURLY},
        Lex::Token {Lex::Token::RETURN},
        Lex::Token {10},
        Lex::Token {Lex::Token::SEMICOLON},
        Lex::Token {Lex::Token::RCURLY},
        Lex::Token {},
    };
    REQUIRE_THAT(tokens, Equals(expectedTokens));
}

TEST_CASE("Trying to compile a simple `main`", "[Comp::Compiler]") {
    using Catch::Matchers::Equals;

    const auto inputProgram = Tools::loadFile("./test/shortterm_goal.scot");

    Lex::Lexer lex {inputProgram};
    const auto tokens = lex.parseAll();

    Parse::Parser parse {tokens};
    const auto ast = parse.makeProgram();

    Comp::Compiler comp {ast};
    const auto outputCode = comp.compile();

    const auto expectedOutputCode = Tools::loadFile("./test/shortterm_goal_expected_output.wat");

    REQUIRE_THAT(outputCode, Equals(expectedOutputCode));
}
