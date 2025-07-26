#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_RANK_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_RANK_H_

// IWYU pragma: private, include "../rank.h"

#include <cstdint>

namespace chesscxx {
/// @brief Represents the ranks (rows) on a chessboard.
enum class Rank : uint8_t {
  k8,
  k7,
  k6,
  k5,
  k4,
  k3,
  k2,
  k1,
};

/// @defgroup RankNonMemberConstants Rank non-member constants
/// @{

/// @brief The number of ranks (rows) on a chessboard.
inline constexpr uint8_t kNumRanks = 8;

/// @}

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_RANK_H_
