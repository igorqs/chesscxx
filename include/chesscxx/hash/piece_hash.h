#ifndef CHESSCXX_INCLUDE_CHESSCXX_HASH_PIECE_HASH_H_
#define CHESSCXX_INCLUDE_CHESSCXX_HASH_PIECE_HASH_H_

// IWYU pragma: private, include "../piece.h"

#include <cstddef>
#include <functional>

#include "../core/piece.h"
#include "internal/hash_combine.h"

/// @ingroup PieceHelpers
/// @brief hash support for chesscxx::Piece
template <>
struct std::hash<chesscxx::Piece> {
  auto operator()(const chesscxx::Piece& piece) const -> size_t {
    return chesscxx::internal::hashCombine(piece.type, piece.color);
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_HASH_PIECE_HASH_H_
