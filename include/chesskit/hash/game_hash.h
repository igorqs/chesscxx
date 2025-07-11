#ifndef CHESSKIT_INCLUDE_CHESSKIT_HASH_GAME_HASH_H_
#define CHESSKIT_INCLUDE_CHESSKIT_HASH_GAME_HASH_H_

// IWYU pragma: private, include "../game.h"

#include <cstddef>
#include <functional>

#include "../core/game.h"
#include "internal/hash_combine.h"

/// @ingroup GameHelpers
/// @brief hash support for chesskit::Game
template <>
struct std::hash<chesskit::Game> {
  size_t operator()(const chesskit::Game& game) const {
    return chesskit::internal::hashCombine(
        game.initialPosition(),
        chesskit::internal::hashCombineRange(game.uciMoves()));
  };
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_HASH_GAME_HASH_H_
