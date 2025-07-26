#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_CASTLING_RIGHTS_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_CASTLING_RIGHTS_FORMATTER_H_

// IWYU pragma: private, include "../castling_rights.h"

#include <format>
#include <ranges>
#include <string_view>

#include "../core/castling_rights.h"
#include "internal/formatter_helper.h"

/// @ingroup CastlingRightsHelpers
/// @brief formatting support for chesscxx::CastlingRights
template <>
struct std::formatter<chesscxx::CastlingRights> : chesscxx::internal::NoSpec {
  template <typename FmtContext>
  auto format(const chesscxx::CastlingRights& rights, FmtContext& ctx) const {
    constexpr static std::string_view kCastlingSymbols = "KQkq";
    constexpr static auto kSize = static_cast<int>(kCastlingSymbols.size());

    auto out = ctx.out();

    if (rights.none()) return *out++ = '-';

    for (auto right : std::views::iota(0, kSize)) {
      if (rights.toBitset().test(right)) *out++ = kCastlingSymbols[right];
    }

    return out;
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_CASTLING_RIGHTS_FORMATTER_H_
