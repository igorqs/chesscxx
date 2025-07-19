#ifndef CHESSKIT_INCLUDE_CHESSKIT_PARSER_PIECE_PLACEMENT_PARSER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_PARSER_PIECE_PLACEMENT_PARSER_H_

// IWYU pragma: private, include "../piece_placement.h"

#include <cstdint>
#include <expected>
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

namespace chesskit {

namespace internal {
constexpr auto parsePieceArray(const char* begin, const char* end)
    -> std::expected<ParseResult<PiecePlacement::PieceArray, const char*>,
                     ParseError> {
  auto to_char = [](uint8_t digit) -> unsigned char { return digit + '0'; };
  auto from_char = [](char digit_char) -> uint8_t { return digit_char - '0'; };

  PiecePlacement::PieceArray piece_array;
  uint8_t piece_counter = 0;
  std::string_view const str = std::string_view(begin, end);

  for (const auto& rank : str | std::views::split('/')) {
    uint8_t missing_pieces = kNumFiles;

    for (const auto* it = rank.begin(); it < rank.end();) {
      if (missing_pieces == 0) {
        return std::unexpected(ParseError::kInvalidSlashSymbol);
      }

      if (*it >= '1' && *it <= to_char(missing_pieces)) {
        missing_pieces -= from_char(*it);
        piece_counter += from_char(*it);
        ++it;
        if (piece_counter == kNumSquares) {
          return ParseResult{.parsed_value = piece_array, .ptr = it};
        }
        continue;
      }

      auto piece = parseFrom<Piece>(it, rank.end());
      if (!piece) return std::unexpected(piece.error());
      piece_array[piece_counter++] = piece->parsed_value;
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

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_PARSER_PIECE_PLACEMENT_PARSER_H_
