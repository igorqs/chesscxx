#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_SQUARE_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_SQUARE_H_

// IWYU pragma: private, include "../square.h"

#include <cstddef>
#include <cstdint>

#include "../file.h"
#include "../rank.h"
#include "internal/file.h"
#include "internal/rank.h"

namespace chesscxx {
/// @brief Represents a square on a chessboard.
struct Square {
  /// @name Comparison Operators
  /// @{

  /// @brief Equality comparison operator.
  constexpr auto operator==(const Square&) const -> bool;

  /// @}

  /// @brief The file of the square.
  File file{};
  /// @brief The rank of the square.
  Rank rank{};
};

constexpr auto Square::operator==(const Square&) const -> bool = default;

/// @defgroup SquareNonMemberFunctions Square non-member functions
/// @{

/// @brief Computes the linear index of a square on the chessboard.
constexpr auto index(const Square& square) -> size_t {
  return ((internal::index(square.rank) * kNumFiles)) +
         internal::index(square.file);
}

/// @}

/// @defgroup SquareNonMemberConstants Square non-member constants
/// @{

/// @brief The total number of squares on a chessboard.
inline constexpr uint8_t kNumSquares = kNumRanks * kNumFiles;

/// @}

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_SQUARE_H_
