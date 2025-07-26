#include <chesscxx/parse.h>
#include <chesscxx/parse_error.h>
#include <chesscxx/rank.h>
#include <gtest/gtest.h>

#include <array>
#include <bit>
#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_utility.hpp>
#include <string_view>
#include <tuple>

TEST(RankTest, DefaultConstructionResultsInValidRank) {
  chesscxx::Rank const rank{};
  EXPECT_TRUE(magic_enum::enum_contains<chesscxx::Rank>(rank));
}

TEST(RankTest, KNumRanksMatchesEnumCount) {
  EXPECT_EQ(chesscxx::kNumRanks, magic_enum::enum_count<chesscxx::Rank>());
}

TEST(RankTest, KNumRanksIsInBounds) {
  EXPECT_GT(chesscxx::kNumRanks, 0);
  EXPECT_LE(std::bit_width(chesscxx::kNumRanks), 4);
}

TEST(RankTest, RoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesscxx::Rank>([](chesscxx::Rank rank) {
    EXPECT_EQ(rank, chesscxx::parse<chesscxx::Rank>(std::format("{}", rank)));
  });
}

TEST(RankTest, FormatProducesExpectedOutput) {
  using chesscxx::Rank;

  constexpr std::array<std::tuple<chesscxx::Rank, std::string_view>,
                       chesscxx::kNumRanks>
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
  EXPECT_EQ(chesscxx::parse<chesscxx::Rank>("9").error(),
            chesscxx::ParseError::kInvalidRank);
  EXPECT_EQ(chesscxx::parse<chesscxx::Rank>("1 ").error(),
            chesscxx::ParseError::kExpectingEndOfString);
}
