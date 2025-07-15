#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_GAME_RESULT_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_GAME_RESULT_FORMATTER_H_

// IWYU pragma: private, include "../game_result.h"

#include <format>
#include <string_view>
#include <utility>

#include "../core/game_result.h"
#include "internal/formatter_helper.h"

/// @ingroup GameResultHelpers
/// @brief formatting support for chesskit::GameResult
template <>
struct std::formatter<chesskit::GameResult>
    : chesskit::internal::SpecDispatcher<chesskit::internal::VerboseSpec,
                                         chesskit::internal::CompactSpec> {
  auto handleSpec(const auto& result, auto& ctx,
                  chesskit::internal::VerboseSpec) const {
    return std::format_to(ctx.out(), "{}", description(result));
  }

  auto handleSpec(const auto& result, auto& ctx,
                  chesskit::internal::CompactSpec) const {
    return std::format_to(ctx.out(), "{}", compact(result));
  }

 private:
  [[nodiscard]] constexpr auto description(const auto& result) const
      -> std::string_view {
    switch (result) {
      case chesskit::GameResult::kWhiteWins:
        return "white wins";
      case chesskit::GameResult::kBlackWins:
        return "black wins";
      case chesskit::GameResult::kDraw:
        return "draw";
      default:
        std::unreachable();
    }
  }

  [[nodiscard]] constexpr auto compact(const auto& result) const
      -> std::string_view {
    switch (result) {
      case chesskit::GameResult::kWhiteWins:
        return "1-0";
      case chesskit::GameResult::kBlackWins:
        return "0-1";
      case chesskit::GameResult::kDraw:
        return "1/2-1/2";
      default:
        std::unreachable();
    }
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_GAME_RESULT_FORMATTER_H_
