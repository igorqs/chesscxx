#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_MOVE_ERROR_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_MOVE_ERROR_FORMATTER_H_

// IWYU pragma: private, include "../move_error.h"

#include <format>
#include <string_view>
#include <utility>

#include "../core/move_error.h"
#include "internal/formatter_helper.h"

/// @ingroup MoveErrorHelpers
/// @brief formatting support for chesskit::MoveError
template <>
struct std::formatter<chesskit::MoveError> : chesskit::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& error, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}", toString(error));
  }

 private:
  constexpr std::string_view toString(const auto& error) const {
    switch (error) {
      case chesskit::MoveError::kKingOrRookMoved:
        return "King or rook moved";
      case chesskit::MoveError::kKingPathBlocked:
        return "King path blocked";
      case chesskit::MoveError::kRookPathBlocked:
        return "Rook path blocked";
      case chesskit::MoveError::kKingPathUnderAttack:
        return "King path under attack";
      case chesskit::MoveError::kNoValidOrigin:
        return "No valid origin";
      case chesskit::MoveError::kAmbiguousOrigin:
        return "Ambiguous origin";
      case chesskit::MoveError::kMoveLeavesOwnKingInCheck:
        return "Move leaves own king in check";
      case chesskit::MoveError::kIllegalMove:
        return "Illegal move";
      case chesskit::MoveError::kWrongPieceColorAtOrigin:
        return "Wrong piece color at origin";
      case chesskit::MoveError::kNoPieceAtOrigin:
        return "No piece at origin";
      case chesskit::MoveError::kPromotionOnInvalidRank:
        return "Promotion on invalid rank";
      case chesskit::MoveError::kNonPawnPromotionAttempt:
        return "Non pawn promotion attempt";
      case chesskit::MoveError::kMissingPromotionPiece:
        return "Missing promotion piece";
      case chesskit::MoveError::kHalfmoveClockOverflow:
        return "Halfmove clock overflow";
      case chesskit::MoveError::kFullmoveNumberOverflow:
        return "Fullmove number overflow";
      default:
        std::unreachable();
    }
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_MOVE_ERROR_FORMATTER_H_
