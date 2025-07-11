#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_CASTLING_RIGHTS_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_CASTLING_RIGHTS_FORMATTER_H_

// IWYU pragma: private, include "../castling_rights.h"

#include <format>
#include <ranges>
#include <string_view>

#include "../core/castling_rights.h"
#include "internal/formatter_helper.h"

/// @ingroup CastlingRightsHelpers
/// @brief formatting support for chesskit::CastlingRights
template <>
struct std::formatter<chesskit::CastlingRights> : chesskit::internal::NoSpec {
  template <typename FmtContext>
  auto format(const chesskit::CastlingRights& rights, FmtContext& ctx) const {
    constexpr static std::string_view kCastlingSymbols = "KQkq";
    constexpr static auto kSize = static_cast<int>(kCastlingSymbols.size());

    auto out = ctx.out();

    if (rights.none()) return *out++ = '-';

    for (auto i : std::views::iota(0, kSize))
      if (rights.toBitset().test(i)) *out++ = kCastlingSymbols[i];

    return out;
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_CASTLING_RIGHTS_FORMATTER_H_
