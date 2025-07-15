#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_POSITION_ERROR_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_POSITION_ERROR_FORMATTER_H_

// IWYU pragma: private, include "../position_error.h"

#include <format>
#include <string_view>
#include <utility>

#include "../core/position_error.h"
#include "internal/formatter_helper.h"

/// @ingroup PositionErrorHelpers
/// @brief formatting support for chesskit::PositionError
template <>
struct std::formatter<chesskit::PositionError> : chesskit::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& error, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}", toString(error));
  }

 private:
  [[nodiscard]] constexpr auto toString(const auto& error) const
      -> std::string_view {
    switch (error) {
      case chesskit::PositionError::kSideNotToMoveIsUnderAttack:
        return "Side not to move is under attack";
      case chesskit::PositionError::kFullmoveNumberOutOfRange:
        return "Fullmove number out of range";
      case chesskit::PositionError::kHalfmoveClockOutOfRange:
        return "Halfmove clock out of range";
      case chesskit::PositionError::kInvalidCastlingRightsForPiecePositions:
        return "Invalid castling rights for piece positions";
      case chesskit::PositionError::kEnPassantTargetSquareOccupied:
        return "En passant target square occupied";
      case chesskit::PositionError::kEnPassantNoCapturablePawn:
        return "En passant no capturable pawn";
      case chesskit::PositionError::kEnPassantTargetSquareInvalidRank:
        return "En passant target square invalid rank";
      default:
        std::unreachable();
    }
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_POSITION_ERROR_FORMATTER_H_
