#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_CHECK_INDICATOR_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_CHECK_INDICATOR_H_

// IWYU pragma: private, include "../check_indicator.h"

#include <cstdint>

namespace chesskit {
/// @brief Indicates whether a move results in a check or a checkmate.
enum class CheckIndicator : uint8_t {
  kCheck,
  kCheckmate,
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_CHECK_INDICATOR_H_
