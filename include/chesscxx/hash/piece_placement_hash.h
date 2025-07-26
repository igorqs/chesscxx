#ifndef CHESSCXX_INCLUDE_CHESSCXX_HASH_PIECE_PLACEMENT_HASH_H_
#define CHESSCXX_INCLUDE_CHESSCXX_HASH_PIECE_PLACEMENT_HASH_H_

// IWYU pragma: private, include "../piece_placement.h"

#include <cstddef>
#include <functional>

#include "../core/piece_placement.h"
#include "internal/hash_combine.h"

/// @ingroup PiecePlacementHelpers
/// @brief hash support for chesscxx::PiecePlacement
template <>
struct std::hash<chesscxx::PiecePlacement> {
  auto operator()(const chesscxx::PiecePlacement& piece_placement) const
      -> size_t {
    return chesscxx::internal::hashCombineRange(piece_placement.pieceArray());
  };
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_HASH_PIECE_PLACEMENT_HASH_H_
