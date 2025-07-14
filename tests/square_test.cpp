#include <chesskit/file.h>
#include <chesskit/parse.h>
#include <chesskit/parse_error.h>
#include <chesskit/rank.h>
#include <chesskit/square.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <format>
#include <functional>
#include <limits>
#include <magic_enum/magic_enum.hpp>
#include <ranges>
#include <unordered_set>

constexpr auto kAllSquares =
    std::views::cartesian_product(magic_enum::enum_values<chesskit::Rank>(),
                                  magic_enum::enum_values<chesskit::File>()) |
    std::views::transform([](const auto& product) {
      const auto& [rank, file] = product;
      return chesskit::Square(file, rank);
    });

TEST(SquareTest, DefaultConstructionIndexIsEqualZero) {
  chesskit::Square square;
  EXPECT_EQ(chesskit::index(square), 0);
}

TEST(SquareTest, KNumSquaresMatchesCartesianProductCount) {
  EXPECT_EQ(chesskit::kNumSquares, kAllSquares.size());
}

TEST(SquareTest, KNumSquaresIsInBounds) {
  EXPECT_GT(chesskit::kNumSquares, 0);
  EXPECT_LE(chesskit::kNumSquares, std::numeric_limits<uint8_t>::max());
}

TEST(SquareTest, ComparesEqualValuesSuccessfully) {
  std::ranges::for_each(kAllSquares, [](const auto& sq) { EXPECT_EQ(sq, sq); });
}

TEST(SquareTest, ComparesDifferentValuesSuccessfully) {
  constexpr auto kSize = static_cast<int>(kAllSquares.size());

  for (int i : std::views::iota(0, kSize)) {
    for (int j : std::views::iota(0, kSize)) {
      if (i != j) EXPECT_NE(kAllSquares[i], kAllSquares[j]);
    }
  }
}

TEST(SquareTest, IndexOrderIsRowMajor) {
  size_t expected_index = 0;

  std::ranges::for_each(kAllSquares, [&](const auto& sq) {
    EXPECT_EQ(chesskit::index(sq), expected_index++);
  });
}

TEST(SquareTest, HashProducesUniqueValues) {
  std::unordered_set<size_t> hashes;

  std::ranges::for_each(kAllSquares, [&](const auto& sq) {
    EXPECT_TRUE(hashes.insert(std::hash<chesskit::Square>{}(sq)).second);
  });
}

TEST(SquareTest, RoundTripConversionIsSuccessful) {
  std::ranges::for_each(kAllSquares, [](const auto& square) {
    EXPECT_EQ(square,
              chesskit::parse<chesskit::Square>(std::format("{}", square)));
  });
}

TEST(SquareTest, FormatProducesNonEmptyStrings) {
  std::ranges::for_each(kAllSquares, [](const auto& square) {
    EXPECT_FALSE(std::format("{}", square).empty());
  });
}

TEST(SquareTest, FormatProducesExpectedOutput) {
  using chesskit::File;
  using chesskit::Rank;
  using chesskit::Square;
  EXPECT_EQ(std::format("{}", Square(File::kA, Rank::k1)), "a1");
  EXPECT_EQ(std::format("{}", Square(File::kA, Rank::k8)), "a8");
  EXPECT_EQ(std::format("{}", Square(File::kH, Rank::k1)), "h1");
  EXPECT_EQ(std::format("{}", Square(File::kH, Rank::k8)), "h8");
  EXPECT_EQ(std::format("{}", Square(File::kE, Rank::k4)), "e4");
  EXPECT_EQ(std::format("{}", Square(File::kD, Rank::k5)), "d5");
}

TEST(SquareTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesskit::parse<chesskit::Square>("x4").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::Square>("ex").error(),
            chesskit::ParseError::kInvalidRank);
  EXPECT_EQ(chesskit::parse<chesskit::Square>("e4 ").error(),
            chesskit::ParseError::kExpectingEndOfString);
}
