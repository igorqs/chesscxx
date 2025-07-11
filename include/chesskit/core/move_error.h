#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_MOVE_ERROR_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_MOVE_ERROR_H_

// IWYU pragma: private, include "../move_error.h"

#include <cstdint>

namespace chesskit {

/// @brief  Represents various reasons a move may be considered invalid.
enum class MoveError : uint8_t {
  /// Castling failed because the king or rook has already moved.
  kKingOrRookMoved,
  /// Castling is blocked by pieces in the king's path.
  kKingPathBlocked,
  /// Castling is blocked by pieces in the rook's path.
  kRookPathBlocked,
  /// Castling is not allowed because the king would move through or into a
  /// square under attack.
  kKingPathUnderAttack,

  /// No valid origin square found for the specified SAN move.
  kNoValidOrigin,
  /// The origin square specified in SAN notation is ambiguous.
  kAmbiguousOrigin,

  /// The piece at the origin cannot legally move to the destination square
  /// (UCI).
  kIllegalMove,
  /// The piece at the origin does not belong to the player making the move
  /// (UCI).
  kWrongPieceColorAtOrigin,
  /// The origin square is empty (UCI).
  kNoPieceAtOrigin,

  /// The move would leave the player's own king in check.
  kMoveLeavesOwnKingInCheck,

  /// Promotion attempted on a non-final rank.
  kPromotionOnInvalidRank,
  /// Attempted to promote a piece that is not a pawn.
  kNonPawnPromotionAttempt,
  /// Promotion piece was not specified when required.
  kMissingPromotionPiece,

  /// The move would cause the halfmove clock to overflow.
  kHalfmoveClockOverflow,
  /// The move would cause the fullmove number to overflow.
  kFullmoveNumberOverflow,
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_MOVE_ERROR_H_
