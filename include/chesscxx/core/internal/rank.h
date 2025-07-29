#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_RANK_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_RANK_H_

#include <cstdint>
#include <optional>

#include "../../color.h"
#include "../../rank.h"

namespace chesscxx::internal {
constexpr auto index(const Rank& rank) -> uint8_t {
  return static_cast<uint8_t>(rank);
}

constexpr auto toRank(uint8_t index) -> std::optional<Rank> {
  if (index >= kNumRanks) return std::nullopt;

  return static_cast<Rank>(index);
}

constexpr auto isDoublePawnPushTargetRank(const Rank& rank, const Color& color)
    -> bool {
  static constexpr Rank kWhiteDoublePushTargetRank = Rank::k4;
  static constexpr Rank kBlackDoublePushTargetRank = Rank::k5;

  return rank == ((color == Color::kWhite) ? kWhiteDoublePushTargetRank
                                           : kBlackDoublePushTargetRank);
}

constexpr auto isPawnStartingRank(const Rank& rank, const Color& color)
    -> bool {
  static constexpr Rank kWhiteInitialRank = Rank::k2;
  static constexpr Rank kBlackInitialRank = Rank::k7;

  return rank ==
         ((color == Color::kWhite) ? kWhiteInitialRank : kBlackInitialRank);
}

constexpr auto backRank(const Color& color) -> Rank {
  return color == Color::kWhite ? Rank::k1 : Rank::k8;
}

inline auto promotionRank(const Color& color) -> Rank {
  return color == Color::kWhite ? Rank::k8 : Rank::k1;
}

inline auto enPassantRank(const Color& color) -> Rank {
  return color == Color::kWhite ? Rank::k6 : Rank::k3;
}

}  // namespace chesscxx::internal

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_RANK_H_
