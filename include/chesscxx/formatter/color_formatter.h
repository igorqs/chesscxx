#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_COLOR_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_COLOR_FORMATTER_H_

// IWYU pragma: private, include "../color.h"

#include <format>
#include <string_view>

#include "../core/color.h"
#include "internal/formatter_helper.h"

/// @ingroup ColorHelpers
/// @brief formatting support for chesscxx::Color
template <>
struct std::formatter<chesscxx::Color>
    : chesscxx::internal::SpecDispatcher<chesscxx::internal::VerboseSpec,
                                         chesscxx::internal::CompactSpec> {
  auto handleSpec(const auto& color, auto& ctx,
                  chesscxx::internal::VerboseSpec /*unused*/) const {
    return std::format_to(ctx.out(), "{}", description(color));
  }

  auto handleSpec(const auto& color, auto& ctx,
                  chesscxx::internal::CompactSpec /*unused*/) const {
    return std::format_to(ctx.out(), "{}", compact(color));
  }

 private:
  [[nodiscard]] constexpr auto description(const auto& color) const
      -> std::string_view {
    return color == chesscxx::Color::kWhite ? "white" : "black";
  }

  [[nodiscard]] constexpr auto compact(const auto& color) const -> char {
    return color == chesscxx::Color::kWhite ? 'w' : 'b';
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_COLOR_FORMATTER_H_
