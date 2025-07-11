#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_UCI_MOVE_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_UCI_MOVE_FORMATTER_H_

// IWYU pragma: private, include "../uci_move.h"

#include <format>

#include "../core/uci_move.h"
#include "internal/formatter_helper.h"

/// @ingroup UciMoveHelpers
/// @brief formatting support for chesskit::UciMove
template <>
struct std::formatter<chesskit::UciMove> : chesskit::internal::NoSpec {
  template <typename FmtContext>
  auto format(const chesskit::UciMove& uci, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}{}{:[l]}", uci.origin, uci.destination,
                          uci.promotion);
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_UCI_MOVE_FORMATTER_H_
