#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_PIECE_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_PIECE_H_

// IWYU pragma: private, include "../piece.h"

#include "../color.h"
#include "../piece_type.h"

namespace chesskit {

/// @brief Represents a chess piece with a type and color.
struct Piece {
  /// @name Comparison Operators
  /// @{

  /// @brief Equality comparison operator.
  constexpr auto operator==(const Piece&) const -> bool = default;

  /// @}

  /// @brief Type of the chess piece.
  PieceType type{};
  /// @brief Color of the chess piece.
  Color color{};
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_PIECE_H_
