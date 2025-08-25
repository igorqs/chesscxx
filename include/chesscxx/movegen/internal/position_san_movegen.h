#ifndef CHESSCXX_INCLUDE_CHESSCXX_MOVEGEN_INTERNAL_POSITION_SAN_MOVEGEN_H_
#define CHESSCXX_INCLUDE_CHESSCXX_MOVEGEN_INTERNAL_POSITION_SAN_MOVEGEN_H_

#include <generator>
#include <memory>
#include <ranges>

#include "../../core/internal/position_modifier.h"
#include "../../position.h"
#include "../../san_move.h"
#include "position_movegen.h"

namespace chesscxx::internal {

inline auto legalSanMoves(std::shared_ptr<const Position> position)
    -> std::generator<SanMove> {
  using std::ranges::elements_of;

  Position position_copy = *position;

  PositionModifier::resetMoveCounters(position_copy);

  co_yield elements_of(
      legalMoves(position) |
      std::views::transform([&position_copy](const auto& uci) {
        auto expected_record = PositionModifier::move(position_copy, uci);

        auto san = convertTo<SanMove>(*expected_record);

        PositionModifier::undoMove(position_copy, *expected_record);

        return san;
      }));

  co_return;
}

}  // namespace chesscxx::internal

#endif  // CHESSCXX_INCLUDE_CHESSCXX_MOVEGEN_INTERNAL_POSITION_SAN_MOVEGEN_H_
