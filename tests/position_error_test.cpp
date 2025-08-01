#include <chesscxx/position_error.h>
#include <gtest/gtest.h>

#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_utility.hpp>
#include <unordered_set>

TEST(PositionErrorTest, DefaultConstructionResultsInValidPositionError) {
  chesscxx::PositionError const error{};
  EXPECT_TRUE(magic_enum::enum_contains<chesscxx::PositionError>(error));
}

TEST(PositionErrorTest, FormatProducesNonEmptyStrings) {
  magic_enum::enum_for_each<chesscxx::PositionError>(
      [](chesscxx::PositionError position_error) {
        EXPECT_FALSE(std::format("{}", position_error).empty());
      });
}

TEST(PositionErrorTest, FormatProducesUniqueStrings) {
  std::unordered_set<std::string> fmt_strings;

  magic_enum::enum_for_each<chesscxx::PositionError>(
      [&](chesscxx::PositionError position_error) {
        EXPECT_TRUE(
            fmt_strings.insert(std::format("{}", position_error)).second);
      });
}
