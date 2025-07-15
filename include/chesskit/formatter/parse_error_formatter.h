#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PARSE_ERROR_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PARSE_ERROR_FORMATTER_H_

// IWYU pragma: private, include "../parse_error.h"

#include <format>
#include <string_view>
#include <utility>

#include "../parser/parse_error.h"
#include "internal/formatter_helper.h"

/// @ingroup ParseErrorHelpers
/// @brief formatting support for chesskit::ParseError
template <>
struct std::formatter<chesskit::ParseError> : chesskit::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& error, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}", toString(error));
  }

 private:
  [[nodiscard]] constexpr auto toString(const auto& error) const
      -> std::string_view {
    switch (error) {
      case chesskit::ParseError::kInvalidRank:
        return "Invalid rank";
      case chesskit::ParseError::kInvalidFile:
        return "Invalid file";
      case chesskit::ParseError::kInvalidPieceType:
        return "Invalid piece type";
      case chesskit::ParseError::kInvalidPromotablePieceType:
        return "Invalid promotable piece type";
      case chesskit::ParseError::kInvalidSanCastling:
        return "Invalid san castling";
      case chesskit::ParseError::kInvalidSlashSymbol:
        return "Invalid slash symbol";
      case chesskit::ParseError::kMissingPiecePlacementInfo:
        return "Missing piece placement info";
      case chesskit::ParseError::kMissingRankInfo:
        return "Missing rank info";
      case chesskit::ParseError::kInvalidPiece:
        return "Invalid piece";
      case chesskit::ParseError::kInvalidCastlingAvailability:
        return "Invalid castling availability";
      case chesskit::ParseError::kInvalidWhiteSpace:
        return "Invalid white space";
      case chesskit::ParseError::kInvalidDashSymbol:
        return "Invalid dash symbol";
      case chesskit::ParseError::kNumberOutOfRange:
        return "Number out of range";
      case chesskit::ParseError::kInvalidNumber:
        return "Invalid number";
      case chesskit::ParseError::kInvalidColor:
        return "Invalid color";
      case chesskit::ParseError::kInvalidCheckIndicator:
        return "Invalid check indicator";
      case chesskit::ParseError::kInvalidTag:
        return "Invalid tag";
      case chesskit::ParseError::kInvalidQuote:
        return "Invalid quote";
      case chesskit::ParseError::kInvalidRightBracket:
        return "Invalid right bracket";
      case chesskit::ParseError::kInvalidPiecePlacement:
        return "Invalid piece placement";
      case chesskit::ParseError::kInvalidPosition:
        return "Invalid position";
      case chesskit::ParseError::kInvalidGameResult:
        return "Invalid game result";
      case chesskit::ParseError::kInvalidMove:
        return "Invalid move";
      case chesskit::ParseError::kDuplicatedFenTag:
        return "Duplicated fen tag";
      case chesskit::ParseError::kExpectingEndOfString:
        return "Expecting end of string";
      default:
        std::unreachable();
    }
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PARSE_ERROR_FORMATTER_H_
