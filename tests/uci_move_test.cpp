#include <chesscxx/file.h>
#include <chesscxx/parse.h>
#include <chesscxx/parse_error.h>
#include <chesscxx/piece_type.h>
#include <chesscxx/rank.h>
#include <chesscxx/square.h>
#include <chesscxx/uci_move.h>
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

constexpr auto kAllOptPromotions = chesscxx::testing::make_optional_enum_array<
    chesscxx::PromotablePieceType>();

constexpr auto kAllSquares =
    std::views::cartesian_product(magic_enum::enum_values<chesscxx::Rank>(),
                                  magic_enum::enum_values<chesscxx::File>()) |
    std::views::transform([](const auto& product) {
      const auto& [rank, file] = product;
      return chesscxx::Square(file, rank);
    });

constexpr auto kUciMoves =
    std::views::cartesian_product(kAllSquares, kAllSquares, kAllOptPromotions) |
    std::views::transform([](const auto& product) {
      const auto& [origin, destination, promotion] = product;
      return chesscxx::UciMove(origin, destination, promotion);
    });

TEST(UciMoveTest, DefaultConstructionMembersAreDefaultInitialized) {
  chesscxx::UciMove const uci;
  EXPECT_EQ(chesscxx::index(uci.origin), 0);
  EXPECT_EQ(chesscxx::index(uci.destination), 0);
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
    auto insertion = hashes.insert(std::hash<chesscxx::UciMove>{}(uci));
    EXPECT_TRUE(insertion.second);
  });
}

TEST(UciMoveTest, RoundTripConversionIsSuccessful) {
  std::ranges::for_each(kUciMoves, [](const auto& uci) {
    EXPECT_EQ(uci, chesscxx::parse<chesscxx::UciMove>(std::format("{}", uci)));
  });
}

TEST(UciMoveTest, FormatProducesNonEmptyStrings) {
  std::ranges::for_each(kUciMoves, [](const auto& uci) {
    EXPECT_FALSE(std::format("{}", uci).empty());
  });
}

TEST(UciMoveTest, FormatProducesExpectedOutput) {
  using chesscxx::Square;
  using chesscxx::UciMove;
  using enum chesscxx::File;
  using enum chesscxx::Rank;

  EXPECT_EQ(std::format("{}", UciMove(Square(kA, k1), Square(kA, k1))), "a1a1");
  EXPECT_EQ(std::format("{}", UciMove(Square(kA, k1), Square(kH, k8))), "a1h8");
  EXPECT_EQ(std::format("{}", UciMove(Square(kC, k3), Square(kF, k4))), "c3f4");
}

TEST(UciMoveTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesscxx::parse<chesscxx::UciMove>("x").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::UciMove>("ex").error(),
            chesscxx::ParseError::kInvalidRank);
  EXPECT_EQ(chesscxx::parse<chesscxx::UciMove>("e4x").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::UciMove>("e4ex").error(),
            chesscxx::ParseError::kInvalidRank);
  EXPECT_EQ(chesscxx::parse<chesscxx::UciMove>("e4e5x").error(),
            chesscxx::ParseError::kExpectingEndOfString);
  EXPECT_EQ(chesscxx::parse<chesscxx::UciMove>("e4e5Q").error(),
            chesscxx::ParseError::kExpectingEndOfString);
}
