#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_PIECE_PLACEMENT_ERROR_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_PIECE_PLACEMENT_ERROR_H_

// IWYU pragma: private, include "../piece_placement_error.h"

#include <cstdint>

namespace chesscxx {

/// @brief Represents errors that can occur during PiecePlacement creation
enum class PiecePlacementError : uint8_t {
  /// One side is missing its king.
  kMissingKing,
  /// A side has more than one king.
  kMultipleKingsOfSameColor,
  /// A pawn is placed on the back rank.
  kPawnOnBackRank,
  /// A pawn is placed on the promotion rank without being promoted.
  kPawnOnPromotionRank,
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_PIECE_PLACEMENT_ERROR_H_
