#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_RANK_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_RANK_H_

#include <cstdint>
#include <optional>

#include "../../color.h"
#include "../../rank.h"

namespace chesskit::internal {
inline constexpr auto index(const Rank& rank) -> uint8_t {
  return static_cast<uint8_t>(rank);
}

inline constexpr auto toRank(const uint8_t& index) -> std::optional<Rank> {
  if (index >= kNumRanks) return std::nullopt;

  return static_cast<Rank>(index);
}

inline constexpr auto isDoublePawnPushTargetRank(const Rank& rank,
                                                 const Color& color) -> bool {
  static constexpr Rank kWhiteDoublePushTargetRank = Rank::k4;
  static constexpr Rank kBlackDoublePushTargetRank = Rank::k5;

  return rank == ((color == Color::kWhite) ? kWhiteDoublePushTargetRank
                                           : kBlackDoublePushTargetRank);
}

inline constexpr auto isPawnStartingRank(const Rank& rank, const Color& color)
    -> bool {
  static constexpr Rank kWhiteInitialRank = Rank::k2;
  static constexpr Rank kBlackInitialRank = Rank::k7;

  return rank ==
         ((color == Color::kWhite) ? kWhiteInitialRank : kBlackInitialRank);
}

inline constexpr auto backRank(const Color& color) -> Rank {
  return color == Color::kWhite ? Rank::k1 : Rank::k8;
}

inline auto promotionRank(const Color& color) -> Rank {
  return color == Color::kWhite ? Rank::k8 : Rank::k1;
}

inline auto enPassantRank(const Color& color) -> Rank {
  return color == Color::kWhite ? Rank::k6 : Rank::k3;
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_RANK_H_
