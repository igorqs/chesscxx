#include <chesscxx/check_indicator.h>
#include <chesscxx/parse.h>
#include <chesscxx/parse_error.h>
#include <gtest/gtest.h>

#include <array>
#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_utility.hpp>
#include <string_view>
#include <tuple>

TEST(CheckIndicatorTest, DefaultConstructionResultsInValidCheckIndicator) {
  chesscxx::CheckIndicator const check_indicator{};
  EXPECT_TRUE(
      magic_enum::enum_contains<chesscxx::CheckIndicator>(check_indicator));
}

TEST(CheckIndicatorTest, RoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesscxx::CheckIndicator>(
      [](chesscxx::CheckIndicator check_indicator) {
        EXPECT_EQ(check_indicator, chesscxx::parse<chesscxx::CheckIndicator>(
                                       std::format("{:c}", check_indicator)));
      });
}

TEST(CheckIndicatorTest, FormatProducesExpectedOutput) {
  using chesscxx::CheckIndicator;

  constexpr std::array<
      std::tuple<chesscxx::CheckIndicator, std::string_view, std::string_view>,
      magic_enum::enum_count<chesscxx::CheckIndicator>()>
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
  EXPECT_EQ(chesscxx::parse<chesscxx::CheckIndicator>("!").error(),
            chesscxx::ParseError::kInvalidCheckIndicator);
  EXPECT_EQ(chesscxx::parse<chesscxx::CheckIndicator>("+ ").error(),
            chesscxx::ParseError::kExpectingEndOfString);
}
