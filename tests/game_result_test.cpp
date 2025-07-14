#include <chesskit/game_result.h>
#include <chesskit/parse.h>
#include <chesskit/parse_error.h>
#include <gtest/gtest.h>

#include <array>
#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_utility.hpp>
#include <string_view>
#include <tuple>

TEST(GameResultTest, DefaultConstructionResultsInValidGameResult) {
  chesskit::GameResult const game_result{};
  EXPECT_TRUE(magic_enum::enum_contains<chesskit::GameResult>(game_result));
}

TEST(GameResultTest, RoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesskit::GameResult>(
      [](chesskit::GameResult game_result) {
        EXPECT_EQ(game_result, chesskit::parse<chesskit::GameResult>(
                                   std::format("{:c}", game_result)));
      });
}

TEST(GameResultTest, FormatProducesExpectedOutput) {
  using chesskit::GameResult;

  constexpr std::array<
      std::tuple<chesskit::GameResult, std::string_view, std::string_view>,
      magic_enum::enum_count<chesskit::GameResult>()>
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
  EXPECT_EQ(chesskit::parse<chesskit::GameResult>("1").error(),
            chesskit::ParseError::kInvalidGameResult);
  EXPECT_EQ(chesskit::parse<chesskit::GameResult>("0-0").error(),
            chesskit::ParseError::kInvalidGameResult);
  EXPECT_EQ(chesskit::parse<chesskit::GameResult>("1-0 ").error(),
            chesskit::ParseError::kExpectingEndOfString);
}
