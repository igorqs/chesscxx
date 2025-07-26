#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_CASTLING_SIDE_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_CASTLING_SIDE_FORMATTER_H_

// IWYU pragma: private, include "../castling_side.h"

#include <format>
#include <string_view>

#include "../core/castling_side.h"
#include "internal/formatter_helper.h"

/// @ingroup CastlingSideHelpers
/// @brief formatting support for chesscxx::CastlingSide
template <>
struct std::formatter<chesscxx::CastlingSide> : chesscxx::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& error, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}", toString(error));
  }

 private:
  [[nodiscard]] constexpr auto toString(auto side) const -> std::string_view {
    return side == chesscxx::CastlingSide::kKingside ? "kingside" : "queenside";
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_CASTLING_SIDE_FORMATTER_H_
