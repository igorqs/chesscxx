#ifndef CHESSCXX_INCLUDE_CHESSCXX_PARSER_PARSE_ERROR_H_
#define CHESSCXX_INCLUDE_CHESSCXX_PARSER_PARSE_ERROR_H_

// IWYU pragma: private, include "../parse_error.h"

#include <cstdint>

namespace chesscxx {

/// @brief Represents possible errors that may occur during parsing operations.
enum class ParseError : uint8_t {
  /// Invalid rank token.
  kInvalidRank,
  /// Invalid file token.
  kInvalidFile,
  /// Invalid piece type token.
  kInvalidPieceType,
  /// Invalid promotable piece type token.
  kInvalidPromotablePieceType,
  /// Invalid SAN castling notation.
  kInvalidSanCastling,
  /// Invalid slash '/' token.
  kInvalidSlashSymbol,
  /// %Piece placement information is incomplete.
  kMissingPiecePlacementInfo,
  /// %Rank information is incomplete.
  kMissingRankInfo,
  /// Invalid piece token.
  kInvalidPiece,
  /// Invalid castling availability string.
  kInvalidCastlingAvailability,
  /// Invalid white space token
  kInvalidWhiteSpace,
  /// Invalid dash '-' token.
  kInvalidDashSymbol,
  /// Parsed number is out of range.
  kNumberOutOfRange,
  /// Invalid number format.
  kInvalidNumber,
  /// Invalid color token.
  kInvalidColor,
  /// Invalid check indicator token.
  kInvalidCheckIndicator,
  /// Invalid PGN tag.
  kInvalidTag,
  /// Invalid quote '\"' token.
  kInvalidQuote,
  /// Invalid right bracket ']' token.
  kInvalidRightBracket,
  /// Parsed PiecePlacement is invalid. See PiecePlacementError to see possible
  /// causes.
  kInvalidPiecePlacement,
  /// Parsed Position is invalid. See PositionError to see possible causes.
  kInvalidPosition,
  /// Invalid game result string.
  kInvalidGameResult,
  /// Invalid move. See MoveError to see possible causes.
  kInvalidMove,
  /// Duplicated FEN tag.
  kDuplicatedFenTag,
  /// Expected end of string, but found more characters.
  kExpectingEndOfString,
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_PARSER_PARSE_ERROR_H_
