#ifndef CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_INTERNAL_POSITION_EN_PASSANT_MOVEGEN_H_
#define CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_INTERNAL_POSITION_EN_PASSANT_MOVEGEN_H_

#include <generator>
#include <optional>
#include <ranges>

#include "../../core/internal/piece_placement.h"
#include "../../core/internal/raw_move.h"
#include "../../core/position.h"
#include "../../square.h"
#include "piece_placement_movegen.h"

namespace chesskit::internal {

inline auto pseudoLegalEnPassantCaptures(const Position& position)
    -> std::generator<RawMove> {
  if (!position.enPassantTargetSquare()) co_return;

  auto target = *position.enPassantTargetSquare();

  co_yield std::ranges::elements_of(
      pawnsAttacking(position.piecePlacement(), target,
                     position.activeColor()) |
      std::views::transform(
          [&](const Square& origin) { return RawMove(origin, target); }));
}

inline auto legalEnPassantCaptures(const Position& position)
    -> std::generator<RawMove> {
  using std::ranges::elements_of;

  co_yield elements_of(pseudoLegalEnPassantCaptures(position) |
                       std::views::filter([position](const RawMove& move) {
                         return !moveResultsInSelfCheck(
                             position.piecePlacement(), move,
                             position.activeColor());
                       }));
}

inline auto hasLegalEnPassantCapture(const Position& position) -> bool {
  auto captures = legalEnPassantCaptures(position);
  return captures.begin() != captures.end();
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_INTERNAL_POSITION_EN_PASSANT_MOVEGEN_H_
