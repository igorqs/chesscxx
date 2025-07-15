#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_DRAW_REASON_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_DRAW_REASON_FORMATTER_H_

// IWYU pragma: private, include "../draw_reason.h"

#include <format>
#include <string_view>
#include <utility>

#include "../core/draw_reason.h"
#include "internal/formatter_helper.h"

/// @ingroup DrawReasonHelpers
/// @brief formatting support for chesskit::DrawReason
template <>
struct std::formatter<chesskit::DrawReason> : chesskit::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& result, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}", toString(result));
  }

 private:
  [[nodiscard]] constexpr auto toString(const auto& result) const
      -> std::string_view {
    switch (result) {
      case chesskit::DrawReason::kStalemate:
        return "stalemate";
      case chesskit::DrawReason::kFiftyMoveRule:
        return "fifty move rule";
      case chesskit::DrawReason::kInsufficientMaterial:
        return "insufficient material";
      case chesskit::DrawReason::kThreefoldRepetition:
        return "three fold repetition";
      default:
        std::unreachable();
    }
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_DRAW_REASON_FORMATTER_H_
