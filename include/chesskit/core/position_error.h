#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_POSITION_ERROR_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_POSITION_ERROR_H_

// IWYU pragma: private, include "../position_error.h"

#include <cstdint>

namespace chesskit {

/// @brief Represents errors that can occur during Position creation
enum class PositionError : uint8_t {
  /// @brief The king of the player NOT to move is currently attacked by the
  /// player TO move.
  kSideNotToMoveIsUnderAttack,

  /// @brief Fullmove number out of valid range [kMinFullmoveNumber,
  /// kMaxFullmoveNumber].
  kFullmoveNumberOutOfRange,

  /// @brief The halfmove clock value is inconsistent with the current fullmove
  /// number and active color.
  kHalfmoveClockOutOfRange,

  /// @brief The asserted castling rights conflict with the current positions of
  /// the king or rooks.
  kInvalidCastlingRightsForPiecePositions,

  /// @brief An en passant target square is specified, but this square is
  /// currently occupied by a piece.
  kEnPassantTargetSquareOccupied,

  /// @brief An en passant target square is specified, but the opponent's pawn
  /// that supposedly just moved two squares (and would be captured en passant)
  /// is not correctly placed or is not an opponent's pawn.
  kEnPassantNoCapturablePawn,

  /// @brief An en passant target square is specified, but it's on an invalid
  /// rank based on the current active color.
  kEnPassantTargetSquareInvalidRank,
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_POSITION_ERROR_H_
