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
constexpr std::expected<ParseResult<PiecePlacement::PieceArray, const char*>,
                        ParseError>
parsePieceArray(const char* begin, const char* end) {
  auto toChar = [](uint8_t i) -> unsigned char { return i + '0'; };
  auto fromChar = [](char c) -> uint8_t { return c - '0'; };

  PiecePlacement::PieceArray pieceArray;
  uint8_t pieceCounter = 0;
  std::string_view sv = std::string_view(begin, end);

  for (const auto& rank : sv | std::views::split('/')) {
    uint8_t missingPieces = kNumFiles;

    for (auto it = rank.begin(); it < rank.end();) {
      if (missingPieces == 0) {
        return std::unexpected(ParseError::kInvalidSlashSymbol);
      }

      if (*it >= '1' && *it <= toChar(missingPieces)) {
        missingPieces -= fromChar(*it);
        pieceCounter += fromChar(*it);
        ++it;
        if (pieceCounter == kNumSquares) return ParseResult{pieceArray, it};
        continue;
      }

      auto piece = parseFrom<Piece>(it, rank.end());
      if (!piece) return std::unexpected(piece.error());
      pieceArray[pieceCounter++] = piece->parsedValue;
      it = piece->ptr;
      missingPieces--;

      if (pieceCounter == kNumSquares) return ParseResult{pieceArray, it};
    }

    if (missingPieces > 0) return std::unexpected(ParseError::kMissingRankInfo);
  }

  return std::unexpected(ParseError::kMissingPiecePlacementInfo);
}
}  // namespace internal

/// @ingroup PiecePlacementHelpers
/// @brief parsing support for PiecePlacement
template <>
class Parser<PiecePlacement, const char*, parse_as::Default> {
 public:
  constexpr std::expected<ParseResult<PiecePlacement, const char*>, ParseError>
  parse(const char* begin, const char* end) {
    auto array = internal::parsePieceArray(begin, end);
    if (!array) return std::unexpected(array.error());

    auto pp = PiecePlacement::fromPieceArray(array->parsedValue);
    if (!pp) return std::unexpected(ParseError::kInvalidPiecePlacement);

    return ParseResult{pp.value(), array->ptr};
  }
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_PARSER_PIECE_PLACEMENT_PARSER_H_
