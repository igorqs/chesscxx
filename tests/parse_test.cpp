#include <chesskit/parse.h>
#include <chesskit/parse_error.h>
#include <chesskit/piece_type.h>
#include <gtest/gtest.h>

#include <array>
#include <expected>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

struct PortugueseUppercase {};
struct PortugueseLowercase {};

template <>
class chesskit::Parser<chesskit::PieceType, const char*, PortugueseUppercase> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<chesskit::PieceType, const char*>,
                       ParseError> {
    static constexpr std::string_view kAllowed = "PCBTDR";

    if (begin == end) return std::unexpected(ParseError::kInvalidPieceType);

    auto index = kAllowed.find(*begin);

    if (index == std::string_view::npos) {
      return std::unexpected(ParseError::kInvalidPieceType);
    }

    return ParseResult{.parsed_value = static_cast<chesskit::PieceType>(index),
                       .ptr = begin + 1};
  }
};

template <>
class chesskit::Parser<chesskit::PieceType, const char*, PortugueseLowercase> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<chesskit::PieceType, const char*>,
                       ParseError> {
    static constexpr std::string_view kAllowed = "pcbtdr";

    if (begin == end) return std::unexpected(ParseError::kInvalidPieceType);

    auto index = kAllowed.find(*begin);

    if (index == std::string_view::npos) {
      return std::unexpected(ParseError::kInvalidPieceType);
    }

    return ParseResult{.parsed_value = static_cast<chesskit::PieceType>(index),
                       .ptr = begin + 1};
  }
};

static constexpr auto kLowercaseInputs =
    std::array{"p", "c", "b", "t", "d", "r"};
static constexpr auto kUppercaseInputs =
    std::array{"P", "C", "B", "T", "D", "R"};
static constexpr auto kInputsSize = static_cast<int>(kLowercaseInputs.size());
static_assert(kLowercaseInputs.size() == kUppercaseInputs.size());

TEST(ParseTest, ParseFromHandlesValidInputCorrectly) {
  using chesskit::parseFrom;
  using chesskit::PieceType;

  for (int i = 0; i < kInputsSize; i++) {
    std::string_view input = kLowercaseInputs[i];
    auto result =
        parseFrom<PieceType>(input.begin(), input.end(), PortugueseLowercase{});
    EXPECT_EQ(result->parsed_value, magic_enum::enum_cast<PieceType>(i));
    EXPECT_EQ(result->ptr, input.end());

    input = kUppercaseInputs[i];
    result =
        parseFrom<PieceType>(input.begin(), input.end(), PortugueseUppercase{});
    EXPECT_EQ(result->parsed_value, magic_enum::enum_cast<PieceType>(i));
    EXPECT_EQ(result->ptr, input.end());
  }
}

TEST(ParseTest, ParseFromHandlesExtraCharactersSuccessfully) {
  using chesskit::ParseError;
  using chesskit::parseFrom;
  using chesskit::PieceType;

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
  using chesskit::ParseError;
  using chesskit::parseFrom;
  using chesskit::PieceType;

  std::string_view const input = "x";
  auto result =
      parseFrom<PieceType>(input.begin(), input.end(), PortugueseLowercase{});
  EXPECT_EQ(result.error(), ParseError::kInvalidPieceType);

  result =
      parseFrom<PieceType>(input.begin(), input.end(), PortugueseUppercase{});
  EXPECT_EQ(result.error(), ParseError::kInvalidPieceType);
}

TEST(ParseTest, ParseHandlesValidInputCorrectly) {
  using chesskit::parse;
  using chesskit::PieceType;

  for (int i = 0; i < kInputsSize; i++) {
    std::string_view input = kLowercaseInputs[i];
    auto result = parse<PieceType>(input, PortugueseLowercase{});
    EXPECT_EQ(result.value(), magic_enum::enum_cast<PieceType>(i));

    input = kUppercaseInputs[i];
    result = parse<PieceType>(input, PortugueseUppercase{});
    EXPECT_EQ(result.value(), magic_enum::enum_cast<PieceType>(i));
  }
}

TEST(ParseTest, ParseFailsWithUnexpectedCharacters) {
  using chesskit::parse;
  using chesskit::ParseError;
  using chesskit::PieceType;

  std::string_view input = "p ";
  auto result = parse<PieceType>(input, PortugueseLowercase{});
  EXPECT_EQ(result.error(), ParseError::kExpectingEndOfString);

  input = "P ";
  result = parse<PieceType>(input, PortugueseUppercase{});
  EXPECT_EQ(result.error(), ParseError::kExpectingEndOfString);
}

TEST(ParseTest, ParseFailsIfParserFails) {
  using chesskit::parse;
  using chesskit::ParseError;
  using chesskit::PieceType;

  std::string_view const input = "x";
  auto result = parse<PieceType>(input, PortugueseLowercase{});
  EXPECT_EQ(result.error(), ParseError::kInvalidPieceType);

  result = parse<PieceType>(input, PortugueseUppercase{});
  EXPECT_EQ(result.error(), ParseError::kInvalidPieceType);
}
