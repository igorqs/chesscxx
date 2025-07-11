#ifndef CHESSKIT_INCLUDE_CHESSKIT_HASH_PIECE_HASH_H_
#define CHESSKIT_INCLUDE_CHESSKIT_HASH_PIECE_HASH_H_

// IWYU pragma: private, include "../piece.h"

#include <cstddef>
#include <functional>

#include "../core/piece.h"
#include "internal/hash_combine.h"

/// @ingroup PieceHelpers
/// @brief hash support for chesskit::Piece
template <>
struct std::hash<chesskit::Piece> {
  size_t operator()(const chesskit::Piece& piece) const {
    return chesskit::internal::hashCombine(piece.type, piece.color);
  };
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_HASH_PIECE_HASH_H_
