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
  std::expected<ParseResult<chesskit::PieceType, const char*>, ParseError>
  parse(const char* begin, const char* end) {
    static constexpr std::string_view allowed = "PCBTDR";

    if (begin == end) return std::unexpected(ParseError::kInvalidPieceType);

    auto index = allowed.find(*begin);

    if (index == std::string_view::npos) {
      return std::unexpected(ParseError::kInvalidPieceType);
    }

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

    if (index == std::string_view::npos) {
      return std::unexpected(ParseError::kInvalidPieceType);
    }

    return ParseResult{static_cast<chesskit::PieceType>(index), begin + 1};
  }
};

void parsePortugueseAndPrint(std::string_view sv, auto tag) {
  auto piece_type = chesskit::parse<chesskit::PieceType>(sv, tag);
  assert(piece_type);

  std::println("{}", piece_type.value());
}

int main() {
  parsePortugueseAndPrint("C", PortugueseUppercase{});
  parsePortugueseAndPrint("d", PortugueseLowercase{});
}
