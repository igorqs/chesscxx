#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_POSITION_ERROR_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_POSITION_ERROR_FORMATTER_H_

// IWYU pragma: private, include "../position_error.h"

#include <format>
#include <string_view>
#include <utility>

#include "../core/position_error.h"
#include "internal/formatter_helper.h"

/// @ingroup PositionErrorHelpers
/// @brief formatting support for chesscxx::PositionError
template <>
struct std::formatter<chesscxx::PositionError> : chesscxx::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& error, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}", toString(error));
  }

 private:
  [[nodiscard]] constexpr auto toString(const auto& error) const
      -> std::string_view {
    switch (error) {
      case chesscxx::PositionError::kSideNotToMoveIsUnderAttack:
        return "Side not to move is under attack";
      case chesscxx::PositionError::kFullmoveNumberOutOfRange:
        return "Fullmove number out of range";
      case chesscxx::PositionError::kHalfmoveClockOutOfRange:
        return "Halfmove clock out of range";
      case chesscxx::PositionError::kInvalidCastlingRightsForPiecePositions:
        return "Invalid castling rights for piece positions";
      case chesscxx::PositionError::kEnPassantTargetSquareOccupied:
        return "En passant target square occupied";
      case chesscxx::PositionError::kEnPassantNoCapturablePawn:
        return "En passant no capturable pawn";
      case chesscxx::PositionError::kEnPassantTargetSquareInvalidRank:
        return "En passant target square invalid rank";
      default:
        std::unreachable();
    }
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_POSITION_ERROR_FORMATTER_H_
