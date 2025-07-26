#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_PARSE_ERROR_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_PARSE_ERROR_FORMATTER_H_

// IWYU pragma: private, include "../parse_error.h"

#include <format>
#include <string_view>
#include <utility>

#include "../parser/parse_error.h"
#include "internal/formatter_helper.h"

/// @ingroup ParseErrorHelpers
/// @brief formatting support for chesscxx::ParseError
template <>
struct std::formatter<chesscxx::ParseError> : chesscxx::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& error, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}", toString(error));
  }

 private:
  [[nodiscard]] constexpr auto toString(const auto& error) const
      -> std::string_view {
    switch (error) {
      case chesscxx::ParseError::kInvalidRank:
        return "Invalid rank";
      case chesscxx::ParseError::kInvalidFile:
        return "Invalid file";
      case chesscxx::ParseError::kInvalidPieceType:
        return "Invalid piece type";
      case chesscxx::ParseError::kInvalidPromotablePieceType:
        return "Invalid promotable piece type";
      case chesscxx::ParseError::kInvalidSanCastling:
        return "Invalid san castling";
      case chesscxx::ParseError::kInvalidSlashSymbol:
        return "Invalid slash symbol";
      case chesscxx::ParseError::kMissingPiecePlacementInfo:
        return "Missing piece placement info";
      case chesscxx::ParseError::kMissingRankInfo:
        return "Missing rank info";
      case chesscxx::ParseError::kInvalidPiece:
        return "Invalid piece";
      case chesscxx::ParseError::kInvalidCastlingAvailability:
        return "Invalid castling availability";
      case chesscxx::ParseError::kInvalidWhiteSpace:
        return "Invalid white space";
      case chesscxx::ParseError::kInvalidDashSymbol:
        return "Invalid dash symbol";
      case chesscxx::ParseError::kNumberOutOfRange:
        return "Number out of range";
      case chesscxx::ParseError::kInvalidNumber:
        return "Invalid number";
      case chesscxx::ParseError::kInvalidColor:
        return "Invalid color";
      case chesscxx::ParseError::kInvalidCheckIndicator:
        return "Invalid check indicator";
      case chesscxx::ParseError::kInvalidTag:
        return "Invalid tag";
      case chesscxx::ParseError::kInvalidQuote:
        return "Invalid quote";
      case chesscxx::ParseError::kInvalidRightBracket:
        return "Invalid right bracket";
      case chesscxx::ParseError::kInvalidPiecePlacement:
        return "Invalid piece placement";
      case chesscxx::ParseError::kInvalidPosition:
        return "Invalid position";
      case chesscxx::ParseError::kInvalidGameResult:
        return "Invalid game result";
      case chesscxx::ParseError::kInvalidMove:
        return "Invalid move";
      case chesscxx::ParseError::kDuplicatedFenTag:
        return "Duplicated fen tag";
      case chesscxx::ParseError::kExpectingEndOfString:
        return "Expecting end of string";
      default:
        std::unreachable();
    }
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_PARSE_ERROR_FORMATTER_H_
