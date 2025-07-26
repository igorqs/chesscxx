#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_GAME_RESULT_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_GAME_RESULT_FORMATTER_H_

// IWYU pragma: private, include "../game_result.h"

#include <format>
#include <string_view>
#include <utility>

#include "../core/game_result.h"
#include "internal/formatter_helper.h"

/// @ingroup GameResultHelpers
/// @brief formatting support for chesscxx::GameResult
template <>
struct std::formatter<chesscxx::GameResult>
    : chesscxx::internal::SpecDispatcher<chesscxx::internal::VerboseSpec,
                                         chesscxx::internal::CompactSpec> {
  auto handleSpec(const auto& result, auto& ctx,
                  chesscxx::internal::VerboseSpec /*unused*/) const {
    return std::format_to(ctx.out(), "{}", description(result));
  }

  auto handleSpec(const auto& result, auto& ctx,
                  chesscxx::internal::CompactSpec /*unused*/) const {
    return std::format_to(ctx.out(), "{}", compact(result));
  }

 private:
  [[nodiscard]] constexpr auto description(const auto& result) const
      -> std::string_view {
    switch (result) {
      case chesscxx::GameResult::kWhiteWins:
        return "white wins";
      case chesscxx::GameResult::kBlackWins:
        return "black wins";
      case chesscxx::GameResult::kDraw:
        return "draw";
      default:
        std::unreachable();
    }
  }

  [[nodiscard]] constexpr auto compact(const auto& result) const
      -> std::string_view {
    switch (result) {
      case chesscxx::GameResult::kWhiteWins:
        return "1-0";
      case chesscxx::GameResult::kBlackWins:
        return "0-1";
      case chesscxx::GameResult::kDraw:
        return "1/2-1/2";
      default:
        std::unreachable();
    }
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_GAME_RESULT_FORMATTER_H_
