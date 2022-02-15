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
