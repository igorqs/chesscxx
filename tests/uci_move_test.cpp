#include <chesskit/file.h>
#include <chesskit/parse.h>
#include <chesskit/parse_error.h>
#include <chesskit/piece_type.h>
#include <chesskit/rank.h>
#include <chesskit/square.h>
#include <chesskit/uci_move.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <cstddef>
#include <format>
#include <functional>
#include <magic_enum/magic_enum.hpp>
#include <optional>
#include <ranges>
#include <unordered_set>

#include "test_helper.h"

constexpr auto kAllOptPromotions = chesskit::testing::make_optional_enum_array<
    chesskit::PromotablePieceType>();

constexpr auto kAllSquares =
    std::views::cartesian_product(magic_enum::enum_values<chesskit::Rank>(),
                                  magic_enum::enum_values<chesskit::File>()) |
    std::views::transform([](const auto& product) {
      const auto& [rank, file] = product;
      return chesskit::Square(file, rank);
    });

constexpr auto kUciMoves =
    std::views::cartesian_product(kAllSquares, kAllSquares, kAllOptPromotions) |
    std::views::transform([](const auto& product) {
      const auto& [origin, destination, promotion] = product;
      return chesskit::UciMove(origin, destination, promotion);
    });

TEST(UciMoveTest, DefaultConstructionMembersAreDefaultInitialized) {
  chesskit::UciMove const uci;
  EXPECT_EQ(chesskit::index(uci.origin), 0);
  EXPECT_EQ(chesskit::index(uci.destination), 0);
  EXPECT_EQ(uci.promotion, std::nullopt);
}

TEST(UciMoveTest, ComparesEqualValuesSuccessfully) {
  std::ranges::for_each(kUciMoves,
                        [](const auto& uci) { EXPECT_EQ(uci, uci); });
}

TEST(UciMoveTest, ComparesDifferentValuesSuccessfully) {
  for (const auto& other : kUciMoves | std::views::drop(1)) {
    EXPECT_NE(kUciMoves.front(), other);
  }
}

TEST(UciMoveTest, HashProducesUniqueValues) {
  std::unordered_set<size_t> hashes;

  std::ranges::for_each(kUciMoves, [&](const auto& uci) {
    auto insertion = hashes.insert(std::hash<chesskit::UciMove>{}(uci));
    EXPECT_TRUE(insertion.second);
  });
}

TEST(UciMoveTest, RoundTripConversionIsSuccessful) {
  std::ranges::for_each(kUciMoves, [](const auto& uci) {
    EXPECT_EQ(uci, chesskit::parse<chesskit::UciMove>(std::format("{}", uci)));
  });
}

TEST(UciMoveTest, FormatProducesNonEmptyStrings) {
  std::ranges::for_each(kUciMoves, [](const auto& uci) {
    EXPECT_FALSE(std::format("{}", uci).empty());
  });
}

TEST(UciMoveTest, FormatProducesExpectedOutput) {
  using chesskit::Square;
  using chesskit::UciMove;
  using enum chesskit::File;
  using enum chesskit::Rank;

  EXPECT_EQ(std::format("{}", UciMove(Square(kA, k1), Square(kA, k1))), "a1a1");
  EXPECT_EQ(std::format("{}", UciMove(Square(kA, k1), Square(kH, k8))), "a1h8");
  EXPECT_EQ(std::format("{}", UciMove(Square(kC, k3), Square(kF, k4))), "c3f4");
}

TEST(UciMoveTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesskit::parse<chesskit::UciMove>("x").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::UciMove>("ex").error(),
            chesskit::ParseError::kInvalidRank);
  EXPECT_EQ(chesskit::parse<chesskit::UciMove>("e4x").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::UciMove>("e4ex").error(),
            chesskit::ParseError::kInvalidRank);
  EXPECT_EQ(chesskit::parse<chesskit::UciMove>("e4e5x").error(),
            chesskit::ParseError::kExpectingEndOfString);
  EXPECT_EQ(chesskit::parse<chesskit::UciMove>("e4e5Q").error(),
            chesskit::ParseError::kExpectingEndOfString);
}
