#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_GAME_RESULT_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_GAME_RESULT_H_

// IWYU pragma: private, include "../game_result.h"

#include <cstdint>

namespace chesskit {
/// @brief Represents possible outcomes of a chess game.
enum class GameResult : uint8_t {
  kWhiteWins,
  kBlackWins,
  kDraw,
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_GAME_RESULT_H_
