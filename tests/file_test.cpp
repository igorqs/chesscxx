#include <chesskit/file.h>
#include <chesskit/parse.h>
#include <chesskit/parse_error.h>
#include <gtest/gtest.h>

#include <array>
#include <bit>
#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_utility.hpp>
#include <string_view>
#include <tuple>

TEST(FileTest, DefaultConstructionResultsInValidFile) {
  chesskit::File file{};
  EXPECT_TRUE(magic_enum::enum_contains<chesskit::File>(file));
}

TEST(FileTest, KNumFilesMatchesEnumCount) {
  EXPECT_EQ(chesskit::kNumFiles, magic_enum::enum_count<chesskit::File>());
}

TEST(FileTest, KNumFilesIsInBounds) {
  EXPECT_GT(chesskit::kNumFiles, 0);
  EXPECT_LE(std::bit_width(chesskit::kNumFiles), 4);
}

TEST(FileTest, RoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesskit::File>([](chesskit::File file) {
    EXPECT_EQ(file, chesskit::parse<chesskit::File>(std::format("{}", file)));
  });
}

TEST(FileTest, FormatProducesExpectedOutput) {
  using chesskit::File;

  constexpr std::array<std::tuple<chesskit::File, std::string_view>,
                       chesskit::kNumFiles>
      kFixtures = {{
          {File::kA, "a"},
          {File::kB, "b"},
          {File::kC, "c"},
          {File::kD, "d"},
          {File::kE, "e"},
          {File::kF, "f"},
          {File::kG, "g"},
          {File::kH, "h"},
      }};

  for (const auto& [rank, sv] : kFixtures) {
    EXPECT_EQ(std::format("{}", rank), sv);
  }
}

TEST(FileTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesskit::parse<chesskit::File>("i").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::File>("a ").error(),
            chesskit::ParseError::kExpectingEndOfString);
}
