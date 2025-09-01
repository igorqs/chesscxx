#ifndef CHESSCXX_INCLUDE_CHESSCXX_MOVEGEN_INTERNAL_POSITION_EN_PASSANT_MOVEGEN_H_
#define CHESSCXX_INCLUDE_CHESSCXX_MOVEGEN_INTERNAL_POSITION_EN_PASSANT_MOVEGEN_H_

#include <generator>
#include <optional>
#include <ranges>
#include <utility>

#include "../../core/internal/piece_placement.h"
#include "../../core/internal/raw_move.h"
#include "../../core/internal/square.h"
#include "../../core/position.h"
#include "../../square.h"
#include "piece_placement_movegen.h"

namespace chesscxx::internal {

inline auto pseudoLegalEnPassantCaptures(Position position)
    -> std::generator<RawMove> {
  const auto& en_passant_target_square = position.enPassantTargetSquare();
  if (!en_passant_target_square) co_return;

  auto target = *en_passant_target_square;

  co_yield std::ranges::elements_of(
      pawnsAttacking(position.piecePlacement(), target,
                     position.activeColor()) |
      std::views::transform(
          [&](const Square& origin) { return RawMove(origin, target); }));
}

inline auto legalEnPassantCaptures(Position position)
    -> std::generator<RawMove> {
  using std::ranges::elements_of;

  co_yield elements_of(pseudoLegalEnPassantCaptures(position) |
                       std::views::filter([position](const RawMove& move) {
                         auto captured_pawn_square =
                             enPassantCapturedPawnSquare(
                                 move.destination, position.activeColor());

                         if (captured_pawn_square) {
                           return !enPassantCaptureResultsInSelfCheck(
                               position.piecePlacement(), move,
                               *captured_pawn_square, position.activeColor());
                         }

                         std::unreachable();
                       }));
}

inline auto hasLegalEnPassantCapture(const Position& position) -> bool {
  auto captures = legalEnPassantCaptures(position);
  return captures.begin() != captures.end();
}

}  // namespace chesscxx::internal

#endif  // CHESSCXX_INCLUDE_CHESSCXX_MOVEGEN_INTERNAL_POSITION_EN_PASSANT_MOVEGEN_H_
