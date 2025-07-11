#include <chesskit/piece_placement_error.h>
#include <gtest/gtest.h>

#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_utility.hpp>
#include <unordered_set>

TEST(PiecePlacementErrorTest,
     DefaultConstructionResultsInValidPiecePlacementError) {
  chesskit::PiecePlacementError error{};
  EXPECT_TRUE(magic_enum::enum_contains<chesskit::PiecePlacementError>(error));
}

TEST(PiecePlacementErrorTest, FormatProducesNonEmptyStrings) {
  magic_enum::enum_for_each<chesskit::PiecePlacementError>(
      [](chesskit::PiecePlacementError piece_placement_error) {
        EXPECT_FALSE(std::format("{}", piece_placement_error).empty());
      });
}

TEST(PiecePlacementErrorTest, FormatProducesUniqueStrings) {
  std::unordered_set<std::string> fmt_strings;

  magic_enum::enum_for_each<chesskit::PiecePlacementError>(
      [&](chesskit::PiecePlacementError piece_placement_error) {
        EXPECT_TRUE(fmt_strings.insert(std::format("{}", piece_placement_error))
                        .second);
      });
}
