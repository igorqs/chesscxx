#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_RANK_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_RANK_FORMATTER_H_

// IWYU pragma: private, include "../rank.h"

#include <cstdint>
#include <format>

#include "../core/rank.h"
#include "internal/formatter_helper.h"

/// @ingroup RankHelpers
/// @brief formatting support for chesscxx::Rank
template <>
struct std::formatter<chesscxx::Rank> : chesscxx::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& rank, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}", compact(rank));
  }

 private:
  [[nodiscard]] constexpr auto compact(const auto& rank) const -> char {
    return static_cast<char>('8' - static_cast<uint8_t>(rank));
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_RANK_FORMATTER_H_
