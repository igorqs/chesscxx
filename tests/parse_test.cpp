#include <chesscxx/parse.h>
#include <chesscxx/parse_error.h>
#include <chesscxx/piece_type.h>
#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <expected>
#include <iterator>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

struct PortugueseUppercase {};
struct PortugueseLowercase {};

template <>
class chesscxx::Parser<chesscxx::PieceType, const char*, PortugueseUppercase> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<chesscxx::PieceType, const char*>,
                       ParseError> {
    static constexpr std::string_view kAllowed = "PCBTDR";

    if (begin == end) return std::unexpected(ParseError::kInvalidPieceType);

    auto index = kAllowed.find(*begin);

    if (index == std::string_view::npos) {
      return std::unexpected(ParseError::kInvalidPieceType);
    }

    return ParseResult{.parsed_value = static_cast<chesscxx::PieceType>(index),
                       .ptr = std::next(begin)};
  }
};

template <>
class chesscxx::Parser<chesscxx::PieceType, const char*, PortugueseLowercase> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<chesscxx::PieceType, const char*>,
                       ParseError> {
    static constexpr std::string_view kAllowed = "pcbtdr";

    if (begin == end) return std::unexpected(ParseError::kInvalidPieceType);

    auto index = kAllowed.find(*begin);

    if (index == std::string_view::npos) {
      return std::unexpected(ParseError::kInvalidPieceType);
    }

    return ParseResult{.parsed_value = static_cast<chesscxx::PieceType>(index),
                       .ptr = std::next(begin)};
  }
};

static constexpr auto kLowercaseInputs =
    std::array{"p", "c", "b", "t", "d", "r"};
static constexpr auto kUppercaseInputs =
    std::array{"P", "C", "B", "T", "D", "R"};
static constexpr auto kInputsSize = static_cast<int>(kLowercaseInputs.size());
static_assert(kLowercaseInputs.size() == kUppercaseInputs.size());

TEST(ParseTest, ParseFromHandlesValidInputCorrectly) {
  using chesscxx::parseFrom;
  using chesscxx::PieceType;

  for (uint8_t i = 0; i < kInputsSize; i++) {
    std::string_view input = kLowercaseInputs.at(i);
    auto result =
        parseFrom<PieceType>(input.begin(), input.end(), PortugueseLowercase{});
    EXPECT_EQ(result->parsed_value, magic_enum::enum_cast<PieceType>(i));
    EXPECT_EQ(result->ptr, input.end());

    input = kUppercaseInputs.at(i);
    result =
        parseFrom<PieceType>(input.begin(), input.end(), PortugueseUppercase{});
    EXPECT_EQ(result->parsed_value, magic_enum::enum_cast<PieceType>(i));
    EXPECT_EQ(result->ptr, input.end());
  }
}

TEST(ParseTest, ParseFromHandlesExtraCharactersSuccessfully) {
  using chesscxx::ParseError;
  using chesscxx::parseFrom;
  using chesscxx::PieceType;

  std::string_view input = "p ";
  auto result =
      parseFrom<PieceType>(input.begin(), input.end(), PortugueseLowercase{});
  EXPECT_EQ(result->parsed_value, PieceType::kPawn);
  EXPECT_EQ(result->ptr, input.begin() + 1);
  EXPECT_NE(result->ptr, input.end());

  input = "P ";
  result =
      parseFrom<PieceType>(input.begin(), input.end(), PortugueseUppercase{});
  EXPECT_EQ(result->parsed_value, PieceType::kPawn);
  EXPECT_EQ(result->ptr, input.begin() + 1);
  EXPECT_NE(result->ptr, input.end());
}

TEST(ParseTest, ParseFromFailsIfParserFails) {
  using chesscxx::ParseError;
  using chesscxx::parseFrom;
  using chesscxx::PieceType;

  std::string_view const input = "x";
  auto result =
      parseFrom<PieceType>(input.begin(), input.end(), PortugueseLowercase{});
  EXPECT_EQ(result.error(), ParseError::kInvalidPieceType);

  result =
      parseFrom<PieceType>(input.begin(), input.end(), PortugueseUppercase{});
  EXPECT_EQ(result.error(), ParseError::kInvalidPieceType);
}

TEST(ParseTest, ParseHandlesValidInputCorrectly) {
  using chesscxx::parse;
  using chesscxx::PieceType;

  for (uint8_t i = 0; i < kInputsSize; i++) {
    std::string_view input = kLowercaseInputs.at(i);
    auto result = parse<PieceType>(input, PortugueseLowercase{});
    EXPECT_EQ(result.value(), magic_enum::enum_cast<PieceType>(i));

    input = kUppercaseInputs.at(i);
    result = parse<PieceType>(input, PortugueseUppercase{});
    EXPECT_EQ(result.value(), magic_enum::enum_cast<PieceType>(i));
  }
}

TEST(ParseTest, ParseFailsWithUnexpectedCharacters) {
  using chesscxx::parse;
  using chesscxx::ParseError;
  using chesscxx::PieceType;

  std::string_view input = "p ";
  auto result = parse<PieceType>(input, PortugueseLowercase{});
  EXPECT_EQ(result.error(), ParseError::kExpectingEndOfString);

  input = "P ";
  result = parse<PieceType>(input, PortugueseUppercase{});
  EXPECT_EQ(result.error(), ParseError::kExpectingEndOfString);
}

TEST(ParseTest, ParseFailsIfParserFails) {
  using chesscxx::parse;
  using chesscxx::ParseError;
  using chesscxx::PieceType;

  std::string_view const input = "x";
  auto result = parse<PieceType>(input, PortugueseLowercase{});
  EXPECT_EQ(result.error(), ParseError::kInvalidPieceType);

  result = parse<PieceType>(input, PortugueseUppercase{});
  EXPECT_EQ(result.error(), ParseError::kInvalidPieceType);
}
