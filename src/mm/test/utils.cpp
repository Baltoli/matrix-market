#include "catch.h"

#include "utils.h"

#include <iostream>
#include <string_view>

using namespace mm;

TEST_CASE("can left trim strings")
{
  REQUIRE(left_trim("  spaces before") == "spaces before");
  REQUIRE(left_trim("\t\n other before") == "other before");
  REQUIRE(left_trim("none before") == "none before");
  REQUIRE(left_trim("some after  \t\n") == "some after  \t\n");
}

TEST_CASE("can check for prefixes")
{
  SECTION("with valid prefix") {
    auto [match, rest] = starts_with("prefix suffix", "prefix");
    REQUIRE(match);
    REQUIRE(rest == "suffix");
  }

  SECTION("with no prefix match") {
    auto [match, rest] = starts_with("wefj suffix", "prefix");
    REQUIRE(!match);
    REQUIRE(rest == "wefj suffix");
  }
}

TEST_CASE("can iterate over lines in a string")
{
  auto str = R"(
1

2


3

  )";

  int sum = 0;
  for_each_line(str, [&sum] (auto line) {
    sum += std::atoi(line.data());
  });

  REQUIRE(sum == 6);
}
