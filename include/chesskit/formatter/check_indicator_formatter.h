#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_CHECK_INDICATOR_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_CHECK_INDICATOR_FORMATTER_H_

// IWYU pragma: private, include "../check_indicator.h"

#include <format>
#include <string_view>

#include "../core/check_indicator.h"
#include "internal/formatter_helper.h"

/// @ingroup CheckIndicatorHelpers
/// @brief formatting support for chesskit::CheckIndicator
template <>
struct std::formatter<chesskit::CheckIndicator>
    : chesskit::internal::SpecDispatcher<chesskit::internal::VerboseSpec,
                                         chesskit::internal::CompactSpec> {
  auto handleSpec(const auto& check_indicator, auto& ctx,
                  chesskit::internal::VerboseSpec /*unused*/) const {
    return std::format_to(ctx.out(), "{}", description(check_indicator));
  }

  auto handleSpec(const auto& check_indicator, auto& ctx,
                  chesskit::internal::CompactSpec /*unused*/) const {
    return std::format_to(ctx.out(), "{}", compact(check_indicator));
  }

 private:
  [[nodiscard]] constexpr auto description(const auto& check_indicator) const
      -> std::string_view {
    return check_indicator == chesskit::CheckIndicator::kCheck ? "check"
                                                               : "checkmate";
  }

  [[nodiscard]] constexpr auto compact(const auto& check_indicator) const
      -> char {
    return check_indicator == chesskit::CheckIndicator::kCheck ? '+' : '#';
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_CHECK_INDICATOR_FORMATTER_H_
