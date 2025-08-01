#ifndef CHESSCXX_INCLUDE_CHESSCXX_MOVEGEN_INTERNAL_POSITION_SAN_MOVEGEN_H_
#define CHESSCXX_INCLUDE_CHESSCXX_MOVEGEN_INTERNAL_POSITION_SAN_MOVEGEN_H_

#include <generator>
#include <ranges>

#include "../../core/internal/position_modifier.h"
#include "../../position.h"
#include "../../san_move.h"
#include "position_movegen.h"

namespace chesscxx::internal {

inline auto legalSanMoves(Position position) -> std::generator<SanMove> {
  using std::ranges::elements_of;

  PositionModifier::resetMoveCounters(position);

  co_yield elements_of(legalMoves(position) |
                       std::views::transform([&position](const auto& uci) {
                         auto expected_record =
                             PositionModifier::move(position, uci);

                         auto san = convertTo<SanMove>(*expected_record);

                         PositionModifier::undoMove(position, *expected_record);

                         return san;
                       }));

  co_return;
}

}  // namespace chesscxx::internal

#endif  // CHESSCXX_INCLUDE_CHESSCXX_MOVEGEN_INTERNAL_POSITION_SAN_MOVEGEN_H_
