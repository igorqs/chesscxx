#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_DRAW_REASON_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_DRAW_REASON_FORMATTER_H_

// IWYU pragma: private, include "../draw_reason.h"

#include <format>
#include <string_view>
#include <utility>

#include "../core/draw_reason.h"
#include "internal/formatter_helper.h"

/// @ingroup DrawReasonHelpers
/// @brief formatting support for chesscxx::DrawReason
template <>
struct std::formatter<chesscxx::DrawReason> : chesscxx::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& result, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}", toString(result));
  }

 private:
  [[nodiscard]] constexpr auto toString(const auto& result) const
      -> std::string_view {
    switch (result) {
      case chesscxx::DrawReason::kStalemate:
        return "stalemate";
      case chesscxx::DrawReason::kFiftyMoveRule:
        return "fifty move rule";
      case chesscxx::DrawReason::kInsufficientMaterial:
        return "insufficient material";
      case chesscxx::DrawReason::kThreefoldRepetition:
        return "three fold repetition";
      default:
        std::unreachable();
    }
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_DRAW_REASON_FORMATTER_H_
