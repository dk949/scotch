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