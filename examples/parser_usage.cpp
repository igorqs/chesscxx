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
  auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<chesskit::PieceType, const char*>,
                       ParseError> {
    static constexpr std::string_view allowed = "PCBTDR";

    if (begin == end) return std::unexpected(ParseError::kInvalidPieceType);

    auto index = allowed.find(*begin);

    if (index == std::string_view::npos) {
      return std::unexpected(ParseError::kInvalidPieceType);
    }

    return ParseResult{.parsedValue = static_cast<chesskit::PieceType>(index),
                       .ptr = begin + 1};
  }
};

template <>
class chesskit::Parser<chesskit::PieceType, const char*, PortugueseLowercase> {
 public:
  auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<chesskit::PieceType, const char*>,
                       ParseError> {
    static constexpr std::string_view allowed = "pcbtdr";

    if (begin == end) return std::unexpected(ParseError::kInvalidPieceType);

    auto index = allowed.find(*begin);

    if (index == std::string_view::npos) {
      return std::unexpected(ParseError::kInvalidPieceType);
    }

    return ParseResult{.parsedValue = static_cast<chesskit::PieceType>(index),
                       .ptr = begin + 1};
  }
};

namespace {
void parsePortugueseAndPrint(std::string_view sv, auto tag) {
  auto piece_type = chesskit::parse<chesskit::PieceType>(sv, tag);
  assert(piece_type);

  std::println("{}", piece_type.value());
}
}  // namespace

auto main() -> int {
  parsePortugueseAndPrint("C", PortugueseUppercase{});
  parsePortugueseAndPrint("d", PortugueseLowercase{});
}
