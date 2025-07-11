#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_RANK_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_RANK_FORMATTER_H_

// IWYU pragma: private, include "../rank.h"

#include <cstdint>
#include <format>

#include "../core/rank.h"
#include "internal/formatter_helper.h"

/// @ingroup RankHelpers
/// @brief formatting support for chesskit::Rank
template <>
struct std::formatter<chesskit::Rank> : chesskit::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& rank, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}", compact(rank));
  }

 private:
  constexpr char compact(const auto& rank) const {
    return static_cast<char>('8' - static_cast<uint8_t>(rank));
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_RANK_FORMATTER_H_
