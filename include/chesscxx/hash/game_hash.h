#ifndef CHESSCXX_INCLUDE_CHESSCXX_HASH_GAME_HASH_H_
#define CHESSCXX_INCLUDE_CHESSCXX_HASH_GAME_HASH_H_

// IWYU pragma: private, include "../game.h"

#include <cstddef>
#include <functional>

#include "../core/game.h"
#include "internal/hash_combine.h"

/// @ingroup GameHelpers
/// @brief hash support for chesscxx::Game
template <>
struct std::hash<chesscxx::Game> {
  auto operator()(const chesscxx::Game& game) const -> size_t {
    return chesscxx::internal::hashCombine(
        game.initialPosition(),
        chesscxx::internal::hashCombineRange(game.uciMoves()));
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_HASH_GAME_HASH_H_
