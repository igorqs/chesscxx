#ifndef CHESSKIT_INCLUDE_CHESSKIT_HASH_SQUARE_HASH_H_
#define CHESSKIT_INCLUDE_CHESSKIT_HASH_SQUARE_HASH_H_

// IWYU pragma: private, include "../square.h"

#include <cstddef>
#include <functional>

#include "../core/square.h"

/// @ingroup SquareHelpers
/// @brief hash support for chesskit::Square
template <>
struct std::hash<chesskit::Square> {
  size_t operator()(const chesskit::Square& square) const {
    return chesskit::index(square);
  };
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_HASH_SQUARE_HASH_H_
