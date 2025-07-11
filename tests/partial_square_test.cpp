#include <chesskit/file.h>
#include <chesskit/parse.h>
#include <chesskit/parse_error.h>
#include <chesskit/partial_square.h>
#include <chesskit/rank.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <format>
#include <optional>
#include <ranges>

#include "test_helper.h"

constexpr static auto kAllOptFiles =
    chesskit::testing::make_optional_enum_array<chesskit::File>();

constexpr static auto kAllOptRanks =
    chesskit::testing::make_optional_enum_array<chesskit::Rank>();

constexpr static auto kAllPartialSquares =
    std::views::cartesian_product(kAllOptRanks, kAllOptFiles) |
    std::views::transform([](const auto& product) {
      const auto& [rank, file] = product;
      return chesskit::PartialSquare(file, rank);
    });

TEST(PartialSquareTest, DefaultConstructionResultInEmptyMembers) {
  chesskit::PartialSquare partial;
  EXPECT_EQ(partial.file, std::nullopt);
  EXPECT_EQ(partial.rank, std::nullopt);
}

TEST(PartialSquareTest, RoundTripConversionIsSuccessful) {
  std::ranges::for_each(kAllPartialSquares, [](const auto& square) {
    auto round_trip =
        chesskit::parse<chesskit::PartialSquare>(std::format("{}", square));
    EXPECT_TRUE(round_trip);
    EXPECT_EQ(square.file, round_trip->file);
    EXPECT_EQ(square.rank, round_trip->rank);
  });
}

TEST(PartialSquareTest, FormatProducesExpectedOutput) {
  using chesskit::File;
  using chesskit::PartialSquare;
  using chesskit::Rank;
  EXPECT_EQ(std::format("{}", PartialSquare(File::kA, Rank::k1)), "a1");
  EXPECT_EQ(std::format("{}", PartialSquare(File::kA, std::nullopt)), "a");
  EXPECT_EQ(std::format("{}", PartialSquare(std::nullopt, Rank::k1)), "1");
  EXPECT_EQ(std::format("{}", PartialSquare(std::nullopt, std::nullopt)), "");
  EXPECT_EQ(std::format("{}", PartialSquare(File::kE, Rank::k4)), "e4");
  EXPECT_EQ(std::format("{}", PartialSquare(File::kD, Rank::k5)), "d5");
}

TEST(PartialSquareTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesskit::parse<chesskit::PartialSquare>("x").error(),
            chesskit::ParseError::kExpectingEndOfString);
  EXPECT_EQ(chesskit::parse<chesskit::PartialSquare>("ex").error(),
            chesskit::ParseError::kExpectingEndOfString);
  EXPECT_EQ(chesskit::parse<chesskit::PartialSquare>("e4x").error(),
            chesskit::ParseError::kExpectingEndOfString);
}
