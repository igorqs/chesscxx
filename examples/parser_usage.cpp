#include <chesskit/parse.h>
#include <chesskit/parse_error.h>
#include <chesskit/piece_type.h>

#include <cassert>
#include <expected>
#include <print>
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

namespace {
void parsePortugueseAndPrint(std::string_view str, auto tag) {
  auto piece_type = chesskit::parse<chesskit::PieceType>(str, tag);
  assert(piece_type);

  std::println("{}", piece_type.value());
}
}  // namespace

auto main() -> int {
  parsePortugueseAndPrint("C", PortugueseUppercase{});
  parsePortugueseAndPrint("d", PortugueseLowercase{});
}
