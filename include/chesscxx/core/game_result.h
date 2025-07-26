#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_GAME_RESULT_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_GAME_RESULT_H_

// IWYU pragma: private, include "../game_result.h"

#include <cstdint>

namespace chesscxx {
/// @brief Represents possible outcomes of a chess game.
enum class GameResult : uint8_t {
  kWhiteWins,
  kBlackWins,
  kDraw,
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_GAME_RESULT_H_
