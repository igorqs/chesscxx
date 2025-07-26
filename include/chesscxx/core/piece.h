#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_PIECE_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_PIECE_H_

// IWYU pragma: private, include "../piece.h"

#include "../color.h"
#include "../piece_type.h"

namespace chesscxx {

/// @brief Represents a chess piece with a type and color.
struct Piece {
  /// @name Comparison Operators
  /// @{

  /// @brief Equality comparison operator.
  constexpr auto operator==(const Piece&) const -> bool;

  /// @}

  /// @brief Type of the chess piece.
  PieceType type{};
  /// @brief Color of the chess piece.
  Color color{};
};

constexpr auto Piece::operator==(const Piece&) const -> bool = default;

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_PIECE_H_
