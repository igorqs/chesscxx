#include <chesskit/parse.h>
#include <chesskit/parse_error.h>
#include <chesskit/rank.h>
#include <gtest/gtest.h>

#include <array>
#include <bit>
#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_utility.hpp>
#include <string_view>
#include <tuple>

TEST(RankTest, DefaultConstructionResultsInValidRank) {
  chesskit::Rank const rank{};
  EXPECT_TRUE(magic_enum::enum_contains<chesskit::Rank>(rank));
}

TEST(RankTest, KNumRanksMatchesEnumCount) {
  EXPECT_EQ(chesskit::kNumRanks, magic_enum::enum_count<chesskit::Rank>());
}

TEST(RankTest, KNumRanksIsInBounds) {
  EXPECT_GT(chesskit::kNumRanks, 0);
  EXPECT_LE(std::bit_width(chesskit::kNumRanks), 4);
}

TEST(RankTest, RoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesskit::Rank>([](chesskit::Rank rank) {
    EXPECT_EQ(rank, chesskit::parse<chesskit::Rank>(std::format("{}", rank)));
  });
}

TEST(RankTest, FormatProducesExpectedOutput) {
  using chesskit::Rank;

  constexpr std::array<std::tuple<chesskit::Rank, std::string_view>,
                       chesskit::kNumRanks>
      kFixtures = {{
          {Rank::k1, "1"},
          {Rank::k2, "2"},
          {Rank::k3, "3"},
          {Rank::k4, "4"},
          {Rank::k5, "5"},
          {Rank::k6, "6"},
          {Rank::k7, "7"},
          {Rank::k8, "8"},
      }};

  for (const auto& [rank, str] : kFixtures) {
    EXPECT_EQ(std::format("{}", rank), str);
  }
}

TEST(RankTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesskit::parse<chesskit::Rank>("9").error(),
            chesskit::ParseError::kInvalidRank);
  EXPECT_EQ(chesskit::parse<chesskit::Rank>("1 ").error(),
            chesskit::ParseError::kExpectingEndOfString);
}
