#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_COLOR_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_COLOR_FORMATTER_H_

// IWYU pragma: private, include "../color.h"

#include <format>
#include <string_view>

#include "../core/color.h"
#include "internal/formatter_helper.h"

/// @ingroup ColorHelpers
/// @brief formatting support for chesskit::Color
template <>
struct std::formatter<chesskit::Color>
    : chesskit::internal::SpecDispatcher<chesskit::internal::VerboseSpec,
                                         chesskit::internal::CompactSpec> {
  auto handleSpec(const auto& color, auto& ctx,
                  chesskit::internal::VerboseSpec) const {
    return std::format_to(ctx.out(), "{}", description(color));
  }

  auto handleSpec(const auto& color, auto& ctx,
                  chesskit::internal::CompactSpec) const {
    return std::format_to(ctx.out(), "{}", compact(color));
  }

 private:
  [[nodiscard]] constexpr auto description(const auto& color) const
      -> std::string_view {
    return color == chesskit::Color::kWhite ? "white" : "black";
  }

  [[nodiscard]] constexpr auto compact(const auto& color) const -> char {
    return color == chesskit::Color::kWhite ? 'w' : 'b';
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_COLOR_FORMATTER_H_
