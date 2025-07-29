#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_CASTLING_RIGHTS_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_CASTLING_RIGHTS_FORMATTER_H_

// IWYU pragma: private, include "../castling_rights.h"

#include <cstddef>
#include <format>
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
    static_assert(kSize == chesscxx::CastlingRights::kNumCastlingRights);

    auto out = ctx.out();

    if (rights.none()) return *out++ = '-';

    for (size_t i = 0; i < kSize; ++i) {
      if (rights.toBitset().test(i)) *out++ = kCastlingSymbols[i];
    }

    return out;
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_CASTLING_RIGHTS_FORMATTER_H_
