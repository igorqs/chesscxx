#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_UCI_MOVE_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_UCI_MOVE_H_

// IWYU pragma: private, include "../uci_move.h"

#include <optional>

#include "../piece_type.h"
#include "../square.h"

namespace chesskit {

///  @brief Represents a move in UCI (Universal Chess Interface) notation.
struct UciMove {
  /// @name Comparison Operators
  ///@{

  /// @brief Equality comparison operator.
  constexpr bool operator==(const UciMove&) const = default;

  /// @}

  /// @brief The origin square of the move.
  Square origin;
  /// @brief The destination square of the move.
  Square destination;
  /// @brief Optional promotion piece type.
  std::optional<PromotablePieceType> promotion;
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_UCI_MOVE_H_
