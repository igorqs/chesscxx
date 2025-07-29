#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_SAN_MOVE_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_SAN_MOVE_H_

// IWYU pragma: private, include "../san_move.h"

#include <optional>
#include <variant>

#include "../castling_side.h"
#include "../check_indicator.h"
#include "../partial_square.h"
#include "../piece_type.h"
#include "../square.h"

namespace chesscxx {

/// @defgroup SanMove SanMove related types
/// @{

/// @brief Represents a castling move in Standard Algebraic Notation (SAN).
struct SanCastlingMove {
  /// @brief The side of the castling move.
  CastlingSide side{};
  /// @brief Indicates whether the move gives check or checkmate.
  std::optional<CheckIndicator> check_indicator = std::nullopt;
};

/// @brief Represents a normal (non-castling) chess move in Standard Algebraic
/// Notation (SAN).
struct SanNormalMove {
  /// @brief The type of piece being moved.
  PieceType piece_type{};
  /// @brief The origin square of the move, which may be fully specified,
  /// partially specified (by file or rank), or unspecified.
  PartialSquare origin{};
  /// @brief True if the move captures an opponent's piece.
  bool is_capture = false;
  /// @brief The destination square of the move.
  Square destination;
  /// @brief Optional promotion piece type.
  std::optional<PromotablePieceType> promotion = std::nullopt;
  /// @brief Indicates whether the move gives check or checkmate.
  std::optional<CheckIndicator> check_indicator = std::nullopt;
};

/// @brief A variant representing either a normal move or a castling move in SAN
/// (Standard Algebraic Notation).
using SanMove = std::variant<SanNormalMove, SanCastlingMove>;

/// @}

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_SAN_MOVE_H_
