#include <chesskit/check_indicator.h>
#include <chesskit/parse.h>
#include <chesskit/parse_error.h>
#include <gtest/gtest.h>

#include <array>
#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_utility.hpp>
#include <string_view>
#include <tuple>

TEST(CheckIndicatorTest, DefaultConstructionResultsInValidCheckIndicator) {
  chesskit::CheckIndicator const check_indicator{};
  EXPECT_TRUE(
      magic_enum::enum_contains<chesskit::CheckIndicator>(check_indicator));
}

TEST(CheckIndicatorTest, RoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesskit::CheckIndicator>(
      [](chesskit::CheckIndicator check_indicator) {
        EXPECT_EQ(check_indicator, chesskit::parse<chesskit::CheckIndicator>(
                                       std::format("{:c}", check_indicator)));
      });
}

TEST(CheckIndicatorTest, FormatProducesExpectedOutput) {
  using chesskit::CheckIndicator;

  constexpr std::array<
      std::tuple<chesskit::CheckIndicator, std::string_view, std::string_view>,
      magic_enum::enum_count<chesskit::CheckIndicator>()>
      kFixtures = {{
          {CheckIndicator::kCheck, "+", "check"},
          {CheckIndicator::kCheckmate, "#", "checkmate"},
      }};

  for (const auto& [rank, compact, verbose] : kFixtures) {
    EXPECT_EQ(std::format("{:c}", rank), compact);
    EXPECT_EQ(std::format("{}", rank), verbose);
    EXPECT_EQ(std::format("{:v}", rank), verbose);
  }
}

TEST(CheckIndicatorTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesskit::parse<chesskit::CheckIndicator>("!").error(),
            chesskit::ParseError::kInvalidCheckIndicator);
  EXPECT_EQ(chesskit::parse<chesskit::CheckIndicator>("+ ").error(),
            chesskit::ParseError::kExpectingEndOfString);
}
