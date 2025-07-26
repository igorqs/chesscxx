#include <chesscxx/game_result.h>
#include <chesscxx/parse.h>
#include <chesscxx/parse_error.h>
#include <gtest/gtest.h>

#include <array>
#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_utility.hpp>
#include <string_view>
#include <tuple>

TEST(GameResultTest, DefaultConstructionResultsInValidGameResult) {
  chesscxx::GameResult const game_result{};
  EXPECT_TRUE(magic_enum::enum_contains<chesscxx::GameResult>(game_result));
}

TEST(GameResultTest, RoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesscxx::GameResult>(
      [](chesscxx::GameResult game_result) {
        EXPECT_EQ(game_result, chesscxx::parse<chesscxx::GameResult>(
                                   std::format("{:c}", game_result)));
      });
}

TEST(GameResultTest, FormatProducesExpectedOutput) {
  using chesscxx::GameResult;

  constexpr std::array<
      std::tuple<chesscxx::GameResult, std::string_view, std::string_view>,
      magic_enum::enum_count<chesscxx::GameResult>()>
      kFixtures = {{
          {GameResult::kBlackWins, "0-1", "black wins"},
          {GameResult::kWhiteWins, "1-0", "white wins"},
          {GameResult::kDraw, "1/2-1/2", "draw"},
      }};

  for (const auto& [rank, compact, verbose] : kFixtures) {
    EXPECT_EQ(std::format("{:c}", rank), compact);
    EXPECT_EQ(std::format("{}", rank), verbose);
    EXPECT_EQ(std::format("{:v}", rank), verbose);
  }
}

TEST(GameResultTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesscxx::parse<chesscxx::GameResult>("1").error(),
            chesscxx::ParseError::kInvalidGameResult);
  EXPECT_EQ(chesscxx::parse<chesscxx::GameResult>("0-0").error(),
            chesscxx::ParseError::kInvalidGameResult);
  EXPECT_EQ(chesscxx::parse<chesscxx::GameResult>("1-0 ").error(),
            chesscxx::ParseError::kExpectingEndOfString);
}
