#include <chesscxx/color.h>
#include <chesscxx/parse.h>
#include <chesscxx/parse_error.h>
#include <gtest/gtest.h>

#include <array>
#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_utility.hpp>
#include <string_view>
#include <tuple>

TEST(ColorTest, DefaultConstructionResultsInValidColor) {
  chesscxx::Color const color{};
  EXPECT_TRUE(magic_enum::enum_contains<chesscxx::Color>(color));
}

TEST(ColorTest, RoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesscxx::Color>([](chesscxx::Color color) {
    EXPECT_EQ(color,
              chesscxx::parse<chesscxx::Color>(std::format("{:c}", color)));
  });
}

TEST(ColorTest, LogicalNotWorksCorrectly) {
  using chesscxx::Color;
  EXPECT_EQ(!Color::kBlack, Color::kWhite);
  EXPECT_EQ(!Color::kWhite, Color::kBlack);
}

TEST(ColorTest, FormatProducesExpectedOutput) {
  using chesscxx::Color;

  constexpr std::array<
      std::tuple<chesscxx::Color, std::string_view, std::string_view>,
      magic_enum::enum_count<chesscxx::Color>()>
      kFixtures = {{
          {Color::kWhite, "w", "white"},
          {Color::kBlack, "b", "black"},
      }};

  for (const auto& [rank, compact, verbose] : kFixtures) {
    EXPECT_EQ(std::format("{:c}", rank), compact);
    EXPECT_EQ(std::format("{}", rank), verbose);
    EXPECT_EQ(std::format("{:v}", rank), verbose);
  }
}

TEST(ColorTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesscxx::parse<chesscxx::Color>("p").error(),
            chesscxx::ParseError::kInvalidColor);
  EXPECT_EQ(chesscxx::parse<chesscxx::Color>("w ").error(),
            chesscxx::ParseError::kExpectingEndOfString);
}
