#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_DRAW_REASON_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_DRAW_REASON_H_

// IWYU pragma: private, include "../draw_reason.h"

#include <cstdint>

namespace chesskit {
/// @brief Represents the possible reasons a chess game can end in a draw.
enum class DrawReason : uint8_t {
  kStalemate,
  kFiftyMoveRule,
  kInsufficientMaterial,
  kThreefoldRepetition,
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_DRAW_REASON_H_
