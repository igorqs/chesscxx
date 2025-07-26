#include <chesscxx/move_error.h>
#include <gtest/gtest.h>

#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_utility.hpp>
#include <unordered_set>

TEST(MoveErrorTest, DefaultConstructionResultsInValidMoveError) {
  chesscxx::MoveError const move_error{};
  EXPECT_TRUE(magic_enum::enum_contains<chesscxx::MoveError>(move_error));
}

TEST(MoveErrorTest, FormatProducesNonEmptyStrings) {
  magic_enum::enum_for_each<chesscxx::MoveError>(
      [](chesscxx::MoveError move_error) {
        EXPECT_FALSE(std::format("{}", move_error).empty());
      });
}

TEST(MoveErrorTest, FormatProducesUniqueStrings) {
  std::unordered_set<std::string> fmt_strings;

  magic_enum::enum_for_each<chesscxx::MoveError>(
      [&](chesscxx::MoveError move_error) {
        EXPECT_TRUE(fmt_strings.insert(std::format("{}", move_error)).second);
      });
}
