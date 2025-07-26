#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_CHECK_INDICATOR_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_CHECK_INDICATOR_H_

// IWYU pragma: private, include "../check_indicator.h"

#include <cstdint>

namespace chesscxx {
/// @brief Indicates whether a move results in a check or a checkmate.
enum class CheckIndicator : uint8_t {
  kCheck,
  kCheckmate,
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_CHECK_INDICATOR_H_
