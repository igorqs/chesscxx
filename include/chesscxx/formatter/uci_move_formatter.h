#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_UCI_MOVE_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_UCI_MOVE_FORMATTER_H_

// IWYU pragma: private, include "../uci_move.h"

#include <format>

#include "../core/uci_move.h"
#include "internal/formatter_helper.h"

/// @ingroup UciMoveHelpers
/// @brief formatting support for chesscxx::UciMove
template <>
struct std::formatter<chesscxx::UciMove> : chesscxx::internal::NoSpec {
  template <typename FmtContext>
  auto format(const chesscxx::UciMove& uci, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}{}{:[l]}", uci.origin, uci.destination,
                          uci.promotion);
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_UCI_MOVE_FORMATTER_H_
