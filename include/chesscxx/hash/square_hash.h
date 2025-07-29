#ifndef CHESSCXX_INCLUDE_CHESSCXX_HASH_SQUARE_HASH_H_
#define CHESSCXX_INCLUDE_CHESSCXX_HASH_SQUARE_HASH_H_

// IWYU pragma: private, include "../square.h"

#include <cstddef>
#include <functional>

#include "../core/square.h"

/// @ingroup SquareHelpers
/// @brief hash support for chesscxx::Square
template <>
struct std::hash<chesscxx::Square> {
  auto operator()(const chesscxx::Square& square) const -> size_t {
    return chesscxx::index(square);
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_HASH_SQUARE_HASH_H_
