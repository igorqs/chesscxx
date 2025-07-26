#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_MOVE_ERROR_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_MOVE_ERROR_FORMATTER_H_

// IWYU pragma: private, include "../move_error.h"

#include <format>
#include <string_view>
#include <utility>

#include "../core/move_error.h"
#include "internal/formatter_helper.h"

/// @ingroup MoveErrorHelpers
/// @brief formatting support for chesscxx::MoveError
template <>
struct std::formatter<chesscxx::MoveError> : chesscxx::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& error, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}", toString(error));
  }

 private:
  [[nodiscard]] constexpr auto toString(const auto& error) const
      -> std::string_view {
    switch (error) {
      case chesscxx::MoveError::kKingOrRookMoved:
        return "King or rook moved";
      case chesscxx::MoveError::kKingPathBlocked:
        return "King path blocked";
      case chesscxx::MoveError::kRookPathBlocked:
        return "Rook path blocked";
      case chesscxx::MoveError::kKingPathUnderAttack:
        return "King path under attack";
      case chesscxx::MoveError::kNoValidOrigin:
        return "No valid origin";
      case chesscxx::MoveError::kAmbiguousOrigin:
        return "Ambiguous origin";
      case chesscxx::MoveError::kMoveLeavesOwnKingInCheck:
        return "Move leaves own king in check";
      case chesscxx::MoveError::kIllegalMove:
        return "Illegal move";
      case chesscxx::MoveError::kWrongPieceColorAtOrigin:
        return "Wrong piece color at origin";
      case chesscxx::MoveError::kNoPieceAtOrigin:
        return "No piece at origin";
      case chesscxx::MoveError::kPromotionOnInvalidRank:
        return "Promotion on invalid rank";
      case chesscxx::MoveError::kNonPawnPromotionAttempt:
        return "Non pawn promotion attempt";
      case chesscxx::MoveError::kMissingPromotionPiece:
        return "Missing promotion piece";
      case chesscxx::MoveError::kHalfmoveClockOverflow:
        return "Halfmove clock overflow";
      case chesscxx::MoveError::kFullmoveNumberOverflow:
        return "Fullmove number overflow";
      default:
        std::unreachable();
    }
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_MOVE_ERROR_FORMATTER_H_
