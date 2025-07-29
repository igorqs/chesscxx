#ifndef CHESSCXX_INCLUDE_CHESSCXX_PARSER_PIECE_PLACEMENT_PARSER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_PARSER_PIECE_PLACEMENT_PARSER_H_

// IWYU pragma: private, include "../piece_placement.h"

#include <cstddef>
#include <expected>
#include <iterator>
#include <ranges>
#include <string_view>

#include "../core/piece_placement.h"
#include "../file.h"
#include "../parse_error.h"
#include "../piece.h"
#include "../square.h"
#include "parse.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesscxx {

namespace internal {
constexpr auto parsePieceArray(const char* begin, const char* end)
    -> std::expected<ParseResult<PiecePlacement::PieceArray, const char*>,
                     ParseError> {
  auto to_char = [](size_t digit) -> char {
    return static_cast<char>(digit + '0');
  };
  auto from_char = [](char digit_char) -> int { return digit_char - '0'; };

  PiecePlacement::PieceArray piece_array;
  size_t piece_counter = 0;
  std::string_view const str = std::string_view(begin, end);

  for (const auto& rank : str | std::views::split('/')) {
    size_t missing_pieces = kNumFiles;

    for (const auto* it = rank.begin(); it < rank.end();) {
      if (missing_pieces == 0) {
        return std::unexpected(ParseError::kInvalidSlashSymbol);
      }

      if (*it >= '1' && *it <= to_char(missing_pieces)) {
        missing_pieces -= static_cast<size_t>(from_char(*it));
        piece_counter += static_cast<size_t>(from_char(*it));
        std::advance(it, 1);
        if (piece_counter == kNumSquares) {
          return ParseResult{.parsed_value = piece_array, .ptr = it};
        }
        continue;
      }

      auto piece = parseFrom<Piece>(it, rank.end());
      if (!piece) return std::unexpected(piece.error());
      piece_array.at(piece_counter++) = piece->parsed_value;
      it = piece->ptr;
      missing_pieces--;

      if (piece_counter == kNumSquares) {
        return ParseResult{.parsed_value = piece_array, .ptr = it};
      }
    }

    if (missing_pieces > 0) {
      return std::unexpected(ParseError::kMissingRankInfo);
    }
  }

  return std::unexpected(ParseError::kMissingPiecePlacementInfo);
}
}  // namespace internal

/// @ingroup PiecePlacementHelpers
/// @brief parsing support for PiecePlacement
template <>
class Parser<PiecePlacement, const char*, parse_as::Default> {
 public:
  static constexpr auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<PiecePlacement, const char*>, ParseError> {
    auto array = internal::parsePieceArray(begin, end);
    if (!array) return std::unexpected(array.error());

    auto piece_placement = PiecePlacement::fromPieceArray(array->parsed_value);
    if (!piece_placement) {
      return std::unexpected(ParseError::kInvalidPiecePlacement);
    }

    return ParseResult{.parsed_value = piece_placement.value(),
                       .ptr = array->ptr};
  }
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_PARSER_PIECE_PLACEMENT_PARSER_H_
