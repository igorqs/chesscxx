#include <chesskit/parse.h>
#include <chesskit/parse_error.h>
#include <chesskit/piece_type.h>
#include <gtest/gtest.h>

#include <array>
#include <expected>
#include <magic_enum/magic_enum.hpp>
#include <ranges>
#include <string_view>

struct PortugueseUppercase {};
struct PortugueseLowercase {};

template <>
class chesskit::Parser<chesskit::PieceType, const char*, PortugueseUppercase> {
 public:
  std::expected<ParseResult<chesskit::PieceType, const char*>, ParseError>
  parse(const char* begin, const char* end) {
    static constexpr std::string_view allowed = "PCBTDR";

    if (begin == end) return std::unexpected(ParseError::kInvalidPieceType);

    auto index = allowed.find(*begin);

    if (index == std::string_view::npos)
      return std::unexpected(ParseError::kInvalidPieceType);

    return ParseResult{static_cast<chesskit::PieceType>(index), begin + 1};
  }
};

template <>
class chesskit::Parser<chesskit::PieceType, const char*, PortugueseLowercase> {
 public:
  std::expected<ParseResult<chesskit::PieceType, const char*>, ParseError>
  parse(const char* begin, const char* end) {
    static constexpr std::string_view allowed = "pcbtdr";

    if (begin == end) return std::unexpected(ParseError::kInvalidPieceType);

    auto index = allowed.find(*begin);

    if (index == std::string_view::npos)
      return std::unexpected(ParseError::kInvalidPieceType);

    return ParseResult{static_cast<chesskit::PieceType>(index), begin + 1};
  }
};

static constexpr auto lowercase_inputs =
    std::array{"p", "c", "b", "t", "d", "r"};
static constexpr auto uppercase_inputs =
    std::array{"P", "C", "B", "T", "D", "R"};
static constexpr auto kInputsSize = static_cast<int>(lowercase_inputs.size());
static_assert(lowercase_inputs.size() == uppercase_inputs.size());

TEST(ParseTest, ParseFromHandlesValidInputCorrectly) {
  using chesskit::parseFrom;
  using chesskit::PieceType;

  for (auto i : std::views::iota(0, kInputsSize)) {
    std::string_view input = lowercase_inputs[i];
    auto result =
        parseFrom<PieceType>(input.begin(), input.end(), PortugueseLowercase{});
    EXPECT_EQ(result->parsedValue, magic_enum::enum_cast<PieceType>(i));
    EXPECT_EQ(result->ptr, input.end());

    input = uppercase_inputs[i];
    result =
        parseFrom<PieceType>(input.begin(), input.end(), PortugueseUppercase{});
    EXPECT_EQ(result->parsedValue, magic_enum::enum_cast<PieceType>(i));
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
  EXPECT_EQ(result->parsedValue, PieceType::kPawn);
  EXPECT_EQ(result->ptr, input.begin() + 1);
  EXPECT_NE(result->ptr, input.end());

  input = "P ";
  result =
      parseFrom<PieceType>(input.begin(), input.end(), PortugueseUppercase{});
  EXPECT_EQ(result->parsedValue, PieceType::kPawn);
  EXPECT_EQ(result->ptr, input.begin() + 1);
  EXPECT_NE(result->ptr, input.end());
}

TEST(ParseTest, ParseFromFailsIfParserFails) {
  using chesskit::ParseError;
  using chesskit::parseFrom;
  using chesskit::PieceType;

  std::string_view input = "x";
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

  for (auto i : std::views::iota(0, kInputsSize)) {
    std::string_view input = lowercase_inputs[i];
    auto result = parse<PieceType>(input, PortugueseLowercase{});
    EXPECT_EQ(result.value(), magic_enum::enum_cast<PieceType>(i));

    input = uppercase_inputs[i];
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

  std::string_view input = "x";
  auto result = parse<PieceType>(input, PortugueseLowercase{});
  EXPECT_EQ(result.error(), ParseError::kInvalidPieceType);

  result = parse<PieceType>(input, PortugueseUppercase{});
  EXPECT_EQ(result.error(), ParseError::kInvalidPieceType);
}
