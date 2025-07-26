#include <chesscxx/file.h>
#include <chesscxx/parse.h>
#include <chesscxx/parse_error.h>
#include <gtest/gtest.h>

#include <array>
#include <bit>
#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_utility.hpp>
#include <string_view>
#include <tuple>

TEST(FileTest, DefaultConstructionResultsInValidFile) {
  chesscxx::File const file{};
  EXPECT_TRUE(magic_enum::enum_contains<chesscxx::File>(file));
}

TEST(FileTest, KNumFilesMatchesEnumCount) {
  EXPECT_EQ(chesscxx::kNumFiles, magic_enum::enum_count<chesscxx::File>());
}

TEST(FileTest, KNumFilesIsInBounds) {
  EXPECT_GT(chesscxx::kNumFiles, 0);
  EXPECT_LE(std::bit_width(chesscxx::kNumFiles), 4);
}

TEST(FileTest, RoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesscxx::File>([](chesscxx::File file) {
    EXPECT_EQ(file, chesscxx::parse<chesscxx::File>(std::format("{}", file)));
  });
}

TEST(FileTest, FormatProducesExpectedOutput) {
  using chesscxx::File;

  constexpr std::array<std::tuple<chesscxx::File, std::string_view>,
                       chesscxx::kNumFiles>
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

  for (const auto& [rank, str] : kFixtures) {
    EXPECT_EQ(std::format("{}", rank), str);
  }
}

TEST(FileTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesscxx::parse<chesscxx::File>("i").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::File>("a ").error(),
            chesscxx::ParseError::kExpectingEndOfString);
}
