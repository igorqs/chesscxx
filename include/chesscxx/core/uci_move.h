#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_UCI_MOVE_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_UCI_MOVE_H_

// IWYU pragma: private, include "../uci_move.h"

#include <optional>

#include "../piece_type.h"
#include "../square.h"

namespace chesscxx {

///  @brief Represents a move in UCI (Universal Chess Interface) notation.
struct UciMove {
  /// @name Comparison Operators
  ///@{

  /// @brief Equality comparison operator.
  constexpr auto operator==(const UciMove&) const -> bool;

  /// @}

  /// @brief The origin square of the move.
  Square origin;
  /// @brief The destination square of the move.
  Square destination;
  /// @brief Optional promotion piece type.
  std::optional<PromotablePieceType> promotion;
};

constexpr auto UciMove::operator==(const UciMove&) const -> bool = default;

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_UCI_MOVE_H_
