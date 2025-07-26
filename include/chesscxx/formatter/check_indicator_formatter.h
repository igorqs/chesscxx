#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_CHECK_INDICATOR_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_CHECK_INDICATOR_FORMATTER_H_

// IWYU pragma: private, include "../check_indicator.h"

#include <format>
#include <string_view>

#include "../core/check_indicator.h"
#include "internal/formatter_helper.h"

/// @ingroup CheckIndicatorHelpers
/// @brief formatting support for chesscxx::CheckIndicator
template <>
struct std::formatter<chesscxx::CheckIndicator>
    : chesscxx::internal::SpecDispatcher<chesscxx::internal::VerboseSpec,
                                         chesscxx::internal::CompactSpec> {
  auto handleSpec(const auto& check_indicator, auto& ctx,
                  chesscxx::internal::VerboseSpec /*unused*/) const {
    return std::format_to(ctx.out(), "{}", description(check_indicator));
  }

  auto handleSpec(const auto& check_indicator, auto& ctx,
                  chesscxx::internal::CompactSpec /*unused*/) const {
    return std::format_to(ctx.out(), "{}", compact(check_indicator));
  }

 private:
  [[nodiscard]] constexpr auto description(const auto& check_indicator) const
      -> std::string_view {
    return check_indicator == chesscxx::CheckIndicator::kCheck ? "check"
                                                               : "checkmate";
  }

  [[nodiscard]] constexpr auto compact(const auto& check_indicator) const
      -> char {
    return check_indicator == chesscxx::CheckIndicator::kCheck ? '+' : '#';
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_CHECK_INDICATOR_FORMATTER_H_
