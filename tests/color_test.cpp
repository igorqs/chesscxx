#include <chesskit/color.h>
#include <chesskit/parse.h>
#include <chesskit/parse_error.h>
#include <gtest/gtest.h>

#include <array>
#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_utility.hpp>
#include <string_view>
#include <tuple>

TEST(ColorTest, DefaultConstructionResultsInValidColor) {
  chesskit::Color color{};
  EXPECT_TRUE(magic_enum::enum_contains<chesskit::Color>(color));
}

TEST(ColorTest, RoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesskit::Color>([](chesskit::Color color) {
    EXPECT_EQ(color,
              chesskit::parse<chesskit::Color>(std::format("{:c}", color)));
  });
}

TEST(ColorTest, LogicalNotWorksCorrectly) {
  using chesskit::Color;
  EXPECT_EQ(!Color::kBlack, Color::kWhite);
  EXPECT_EQ(!Color::kWhite, Color::kBlack);
}

TEST(ColorTest, FormatProducesExpectedOutput) {
  using chesskit::Color;

  constexpr std::array<
      std::tuple<chesskit::Color, std::string_view, std::string_view>,
      magic_enum::enum_count<chesskit::Color>()>
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
  EXPECT_EQ(chesskit::parse<chesskit::Color>("p").error(),
            chesskit::ParseError::kInvalidColor);
  EXPECT_EQ(chesskit::parse<chesskit::Color>("w ").error(),
            chesskit::ParseError::kExpectingEndOfString);
}
