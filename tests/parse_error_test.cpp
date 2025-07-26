#include <chesscxx/parse_error.h>
#include <gtest/gtest.h>

#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_utility.hpp>
#include <unordered_set>

TEST(ParseErrorTest, DefaultConstructionResultsInValidParseError) {
  chesscxx::ParseError const parse_error{};
  EXPECT_TRUE(magic_enum::enum_contains<chesscxx::ParseError>(parse_error));
}

TEST(ParseErrorTest, FormatProducesNonEmptyStrings) {
  magic_enum::enum_for_each<chesscxx::ParseError>(
      [](chesscxx::ParseError parse_error) {
        EXPECT_FALSE(std::format("{}", parse_error).empty());
      });
}

TEST(ParseErrorTest, FormatProducesUniqueStrings) {
  std::unordered_set<std::string> fmt_strings;

  magic_enum::enum_for_each<chesscxx::ParseError>(
      [&](chesscxx::ParseError parse_error) {
        EXPECT_TRUE(fmt_strings.insert(std::format("{}", parse_error)).second);
      });
}
