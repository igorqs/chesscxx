#ifndef CHESSCXX_INCLUDE_CHESSCXX_MOVEGEN_GAME_MOVEGEN_H_
#define CHESSCXX_INCLUDE_CHESSCXX_MOVEGEN_GAME_MOVEGEN_H_

#include <generator>
#include <memory>
#include <ranges>

#include "../game.h"
#include "../san_move.h"
#include "../uci_move.h"
#include "internal/position_movegen.h"
#include "internal/position_san_movegen.h"

namespace chesscxx {

/// @defgroup MovegenGroup Movegen methods
/// @{

inline auto legalUciMoves(std::shared_ptr<const Game> game)
    -> std::generator<UciMove> {
  const std::shared_ptr<const Position> position(game,
                                                 &game->currentPosition());

  co_yield std::ranges::elements_of(internal::legalMoves(position));
}

/// @brief Generates all legal moves in UCI (Universal Chess Interface) format
/// from the current position in the given game.
/// @param game The game whose current position is used to generate moves.
/// @return A generator yielding legal moves in UCI format.
/// @note The moves are not guaranteed to be generated in any specific order.
inline auto legalUciMoves(Game game) -> std::generator<UciMove> {
  auto shared_ptr = std::make_shared<const Game>(game);

  co_yield std::ranges::elements_of(legalUciMoves(shared_ptr));
}

inline auto legalSanMoves(std::shared_ptr<const Game> game)
    -> std::generator<SanMove> {
  const std::shared_ptr<const Position> position(game,
                                                 &game->currentPosition());

  co_yield std::ranges::elements_of(internal::legalSanMoves(position));
}

/// @brief Generates all legal moves in SAN (Standard Algebraic Notation) format
/// from the current position in the given game.
/// @param game The game whose current position is used to generate moves.
/// @return A generator yielding legal moves in SAN format.
/// @note The moves are not guaranteed to be generated in any specific order.
inline auto legalSanMoves(Game game) -> std::generator<SanMove> {
  auto shared_ptr = std::make_shared<const Game>(game);

  co_yield std::ranges::elements_of(legalSanMoves(shared_ptr));
}

/// @}

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_MOVEGEN_GAME_MOVEGEN_H_
