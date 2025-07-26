#ifndef CHESSCXX_INCLUDE_CHESSCXX_PARSER_CASTLING_RIGHTS_PARSER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_PARSER_CASTLING_RIGHTS_PARSER_H_

// IWYU pragma: private, include "../castling_rights.h"

#include <bitset>
#include <expected>
#include <iterator>
#include <ranges>
#include <string_view>

#include "../core/castling_rights.h"
#include "../parse_error.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesscxx {

/// @ingroup CastlingRightsHelpers
/// @brief parsing support for CastlingRights
template <>
class Parser<CastlingRights, const char*, parse_as::Default> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<CastlingRights, const char*>, ParseError> {
    constexpr static std::string_view kCastlingSymbols = "KQkq";
    constexpr static auto kSize = static_cast<int>(kCastlingSymbols.size());

    if (begin == end) {
      return std::unexpected(ParseError::kInvalidCastlingAvailability);
    }

    std::bitset<CastlingRights::kNumCastlingRights> bits;
    static_assert(kSize == bits.size());

    if (*begin == '-') {
      return ParseResult{.parsed_value = CastlingRights(bits),
                         .ptr = std::next(begin)};
    }

    for (auto right : std::views::iota(0, kSize)) {
      if (*begin == kCastlingSymbols[right]) {
        bits.set(right);
        std::advance(begin, 1);
      }
    }

    if (bits.none()) {
      return std::unexpected(ParseError::kInvalidCastlingAvailability);
    }

    return ParseResult{.parsed_value = CastlingRights(bits), .ptr = begin};
  }
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_PARSER_CASTLING_RIGHTS_PARSER_H_
