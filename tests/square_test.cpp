#include <chesscxx/file.h>
#include <chesscxx/parse.h>
#include <chesscxx/parse_error.h>
#include <chesscxx/rank.h>
#include <chesscxx/square.h>
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
    std::views::cartesian_product(magic_enum::enum_values<chesscxx::Rank>(),
                                  magic_enum::enum_values<chesscxx::File>()) |
    std::views::transform([](const auto& product) {
      const auto& [rank, file] = product;
      return chesscxx::Square(file, rank);
    });

TEST(SquareTest, DefaultConstructionIndexIsEqualZero) {
  chesscxx::Square const square;
  EXPECT_EQ(chesscxx::index(square), 0);
}

TEST(SquareTest, KNumSquaresMatchesCartesianProductCount) {
  EXPECT_EQ(chesscxx::kNumSquares, kAllSquares.size());
}

TEST(SquareTest, KNumSquaresIsInBounds) {
  EXPECT_GT(chesscxx::kNumSquares, 0);
  EXPECT_LE(chesscxx::kNumSquares, std::numeric_limits<uint8_t>::max());
}

TEST(SquareTest, ComparesEqualValuesSuccessfully) {
  std::ranges::for_each(kAllSquares,
                        [](const auto& square) { EXPECT_EQ(square, square); });
}

TEST(SquareTest, ComparesDifferentValuesSuccessfully) {
  constexpr auto kSize = static_cast<int>(kAllSquares.size());

  for (int const lhs : std::views::iota(0, kSize)) {
    for (int const rhs : std::views::iota(0, kSize)) {
      if (lhs != rhs) {
        EXPECT_NE(kAllSquares[lhs], kAllSquares[rhs]);
      }
    }
  }
}

TEST(SquareTest, IndexOrderIsRowMajor) {
  size_t expected_index = 0;

  std::ranges::for_each(kAllSquares, [&](const auto& square) {
    EXPECT_EQ(chesscxx::index(square), expected_index++);
  });
}

TEST(SquareTest, HashProducesUniqueValues) {
  std::unordered_set<size_t> hashes;

  std::ranges::for_each(kAllSquares, [&](const auto& square) {
    EXPECT_TRUE(hashes.insert(std::hash<chesscxx::Square>{}(square)).second);
  });
}

TEST(SquareTest, RoundTripConversionIsSuccessful) {
  std::ranges::for_each(kAllSquares, [](const auto& square) {
    EXPECT_EQ(square,
              chesscxx::parse<chesscxx::Square>(std::format("{}", square)));
  });
}

TEST(SquareTest, FormatProducesNonEmptyStrings) {
  std::ranges::for_each(kAllSquares, [](const auto& square) {
    EXPECT_FALSE(std::format("{}", square).empty());
  });
}

TEST(SquareTest, FormatProducesExpectedOutput) {
  using chesscxx::File;
  using chesscxx::Rank;
  using chesscxx::Square;
  EXPECT_EQ(std::format("{}", Square(File::kA, Rank::k1)), "a1");
  EXPECT_EQ(std::format("{}", Square(File::kA, Rank::k8)), "a8");
  EXPECT_EQ(std::format("{}", Square(File::kH, Rank::k1)), "h1");
  EXPECT_EQ(std::format("{}", Square(File::kH, Rank::k8)), "h8");
  EXPECT_EQ(std::format("{}", Square(File::kE, Rank::k4)), "e4");
  EXPECT_EQ(std::format("{}", Square(File::kD, Rank::k5)), "d5");
}

TEST(SquareTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesscxx::parse<chesscxx::Square>("x4").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::Square>("ex").error(),
            chesscxx::ParseError::kInvalidRank);
  EXPECT_EQ(chesscxx::parse<chesscxx::Square>("e4 ").error(),
            chesscxx::ParseError::kExpectingEndOfString);
}
