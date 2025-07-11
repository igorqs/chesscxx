#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_PARTIAL_SQUARE_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_PARTIAL_SQUARE_H_

// IWYU pragma: private, include "../partial_square.h"

#include <optional>

#include "../file.h"
#include "../rank.h"

namespace chesskit {

/// @brief Represents a partial square, used to indicate the origin of a move in
/// SAN notation
struct PartialSquare {
  /// @brief Optional file of the square.
  std::optional<File> file;
  /// @brief Optional rank of the square.
  std::optional<Rank> rank;
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_PARTIAL_SQUARE_H_
