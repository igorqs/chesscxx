#include <chesscxx/file.h>
#include <chesscxx/parse.h>
#include <chesscxx/parse_error.h>
#include <chesscxx/partial_square.h>
#include <chesscxx/rank.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <format>
#include <optional>
#include <ranges>

#include "test_helper.h"

constexpr static auto kAllOptFiles =
    chesscxx::testing::make_optional_enum_array<chesscxx::File>();

constexpr static auto kAllOptRanks =
    chesscxx::testing::make_optional_enum_array<chesscxx::Rank>();

constexpr static auto kAllPartialSquares =
    std::views::cartesian_product(kAllOptRanks, kAllOptFiles) |
    std::views::transform([](const auto& product) {
      const auto& [rank, file] = product;
      return chesscxx::PartialSquare(file, rank);
    });

TEST(PartialSquareTest, DefaultConstructionResultInEmptyMembers) {
  chesscxx::PartialSquare const partial;
  EXPECT_EQ(partial.file, std::nullopt);
  EXPECT_EQ(partial.rank, std::nullopt);
}

TEST(PartialSquareTest, RoundTripConversionIsSuccessful) {
  std::ranges::for_each(kAllPartialSquares, [](const auto& square) {
    auto round_trip =
        chesscxx::parse<chesscxx::PartialSquare>(std::format("{}", square));
    EXPECT_TRUE(round_trip);
    EXPECT_EQ(square.file, round_trip->file);
    EXPECT_EQ(square.rank, round_trip->rank);
  });
}

TEST(PartialSquareTest, FormatProducesExpectedOutput) {
  using chesscxx::File;
  using chesscxx::PartialSquare;
  using chesscxx::Rank;
  EXPECT_EQ(std::format("{}", PartialSquare(File::kA, Rank::k1)), "a1");
  EXPECT_EQ(std::format("{}", PartialSquare(File::kA, std::nullopt)), "a");
  EXPECT_EQ(std::format("{}", PartialSquare(std::nullopt, Rank::k1)), "1");
  EXPECT_EQ(std::format("{}", PartialSquare(std::nullopt, std::nullopt)), "");
  EXPECT_EQ(std::format("{}", PartialSquare(File::kE, Rank::k4)), "e4");
  EXPECT_EQ(std::format("{}", PartialSquare(File::kD, Rank::k5)), "d5");
}

TEST(PartialSquareTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesscxx::parse<chesscxx::PartialSquare>("x").error(),
            chesscxx::ParseError::kExpectingEndOfString);
  EXPECT_EQ(chesscxx::parse<chesscxx::PartialSquare>("ex").error(),
            chesscxx::ParseError::kExpectingEndOfString);
  EXPECT_EQ(chesscxx::parse<chesscxx::PartialSquare>("e4x").error(),
            chesscxx::ParseError::kExpectingEndOfString);
}
