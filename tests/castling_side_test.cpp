#include <chesscxx/castling_side.h>
#include <gtest/gtest.h>

#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_utility.hpp>
#include <unordered_set>

TEST(CastlingSideTest, DefaultConstructionResultsInValidCastlingSide) {
  chesscxx::CastlingSide const side{};
  EXPECT_TRUE(magic_enum::enum_contains<chesscxx::CastlingSide>(side));
}

TEST(CastlingSideTest, FormatProducesNonEmptyStrings) {
  magic_enum::enum_for_each<chesscxx::CastlingSide>(
      [](chesscxx::CastlingSide castling_side) {
        EXPECT_FALSE(std::format("{}", castling_side).empty());
      });
}

TEST(CastlingSideTest, FormatProducesUniqueStrings) {
  std::unordered_set<std::string> fmt_strings;

  magic_enum::enum_for_each<chesscxx::CastlingSide>(
      [&](chesscxx::CastlingSide castling_side) {
        EXPECT_TRUE(
            fmt_strings.insert(std::format("{}", castling_side)).second);
      });
}
