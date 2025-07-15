#ifndef CHESSKIT_INCLUDE_CHESSKIT_HASH_PIECE_PLACEMENT_HASH_H_
#define CHESSKIT_INCLUDE_CHESSKIT_HASH_PIECE_PLACEMENT_HASH_H_

// IWYU pragma: private, include "../piece_placement.h"

#include <cstddef>
#include <functional>

#include "../core/piece_placement.h"
#include "internal/hash_combine.h"

/// @ingroup PiecePlacementHelpers
/// @brief hash support for chesskit::PiecePlacement
template <>
struct std::hash<chesskit::PiecePlacement> {
  auto operator()(const chesskit::PiecePlacement& piecePlacement) const
      -> size_t {
    return chesskit::internal::hashCombineRange(piecePlacement.pieceArray());
  };
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_HASH_PIECE_PLACEMENT_HASH_H_
