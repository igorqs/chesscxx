#include <chesscxx/draw_reason.h>
#include <gtest/gtest.h>

#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_utility.hpp>
#include <unordered_set>

TEST(DrawReasonTest, DefaultConstructionResultsInValidDrawReason) {
  chesscxx::DrawReason const draw_reason{};
  EXPECT_TRUE(magic_enum::enum_contains<chesscxx::DrawReason>(draw_reason));
}

TEST(DrawReasonTest, FormatProducesNonEmptyStrings) {
  magic_enum::enum_for_each<chesscxx::DrawReason>(
      [](chesscxx::DrawReason draw_reason) {
        EXPECT_FALSE(std::format("{}", draw_reason).empty());
      });
}

TEST(DrawReasonTest, FormatProducesUniqueStrings) {
  std::unordered_set<std::string> fmt_strings;

  magic_enum::enum_for_each<chesscxx::DrawReason>(
      [&](chesscxx::DrawReason draw_reason) {
        EXPECT_TRUE(fmt_strings.insert(std::format("{}", draw_reason)).second);
      });
}
