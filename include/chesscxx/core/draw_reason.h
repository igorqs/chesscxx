#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_DRAW_REASON_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_DRAW_REASON_H_

// IWYU pragma: private, include "../draw_reason.h"

#include <cstdint>

namespace chesscxx {
/// @brief Represents the possible reasons a chess game can end in a draw.
enum class DrawReason : uint8_t {
  kStalemate,
  kFiftyMoveRule,
  kInsufficientMaterial,
  kThreefoldRepetition,
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_DRAW_REASON_H_
