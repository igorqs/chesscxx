#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_CASTLING_SIDE_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_CASTLING_SIDE_FORMATTER_H_

// IWYU pragma: private, include "../castling_side.h"

#include <format>
#include <string_view>

#include "../core/castling_side.h"
#include "internal/formatter_helper.h"

/// @ingroup CastlingSideHelpers
/// @brief formatting support for chesskit::CastlingSide
template <>
struct std::formatter<chesskit::CastlingSide> : chesskit::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& error, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}", toString(error));
  }

 private:
  [[nodiscard]] constexpr auto toString(auto side) const -> std::string_view {
    return side == chesskit::CastlingSide::kKingside ? "kingside" : "queenside";
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_CASTLING_SIDE_FORMATTER_H_
