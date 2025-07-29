#include <chesscxx/parse.h>
#include <chesscxx/parse_error.h>
#include <chesscxx/piece_type.h>

#include <cstdlib>
#include <expected>
#include <iterator>
#include <print>
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

namespace {
void verify(bool check) {
  if (!check) std::abort();
}
void parsePortugueseAndPrint(std::string_view str, auto tag) {
  auto piece_type = chesscxx::parse<chesscxx::PieceType>(str, tag);
  verify(piece_type.has_value());

  std::println("{}", piece_type.value());
}
}  // namespace

auto main() -> int {
  parsePortugueseAndPrint("C", PortugueseUppercase{});
  parsePortugueseAndPrint("d", PortugueseLowercase{});
}
