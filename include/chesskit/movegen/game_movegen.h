#ifndef CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_GAME_MOVEGEN_H_
#define CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_GAME_MOVEGEN_H_

#include <generator>

#include "../game.h"
#include "../san_move.h"
#include "../uci_move.h"
#include "internal/position_movegen.h"
#include "internal/position_san_movegen.h"

namespace chesskit {

/// @defgroup MovegenGroup Movegen methods
/// @{

/// @brief Generates all legal moves in UCI (Universal Chess Interface) format
/// from the current position in the given game.
/// @param game The game whose current position is used to generate moves.
/// @return A generator yielding legal moves in UCI format.
/// @note The moves are not guaranteed to be generated in any specific order.
inline auto legalUciMoves(const Game& game) -> std::generator<UciMove> {
  return internal::legalMoves(game.currentPosition());
}

/// @brief Generates all legal moves in SAN (Standard Algebraic Notation) format
/// from the current position in the given game.
/// @param game The game whose current position is used to generate moves.
/// @return A generator yielding legal moves in SAN format.
/// @note The moves are not guaranteed to be generated in any specific order.
inline auto legalSanMoves(const Game& game) -> std::generator<SanMove> {
  return internal::legalSanMoves(game.currentPosition());
}

/// @}

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_GAME_MOVEGEN_H_
