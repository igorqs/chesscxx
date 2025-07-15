#ifndef CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_INTERNAL_POSITION_SAN_MOVEGEN_H_
#define CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_INTERNAL_POSITION_SAN_MOVEGEN_H_

#include <generator>
#include <ranges>

#include "../../core/internal/position_modifier.h"
#include "../../position.h"
#include "../../san_move.h"
#include "../../uci_move.h"
#include "position_movegen.h"

namespace chesskit::internal {

template <>
inline auto legalMoves(Position position) -> std::generator<SanMove> {
  using std::ranges::elements_of;

  PositionModifier::resetMoveCounters(position);

  co_yield elements_of(legalMoves<UciMove>(position) |
                       std::views::transform([&position](const auto& uci) {
                         auto expected_record =
                             PositionModifier::move(position, uci);

                         auto san = convertTo<SanMove>(*expected_record);

                         PositionModifier::undoMove(position, *expected_record);

                         return san;
                       }));

  co_return;
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_INTERNAL_POSITION_SAN_MOVEGEN_H_
