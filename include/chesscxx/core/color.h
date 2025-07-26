#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_COLOR_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_COLOR_H_

// IWYU pragma: private, include "../color.h"

#include <cstdint>

namespace chesscxx {
/// @brief Represents colors used in chess.
enum class Color : uint8_t {
  kWhite,
  kBlack,
};

/// @defgroup ColorNonMemberFunctions Color non-member functions
/// @{

/// @brief Returns the opposite color.
inline auto operator!(const Color& color) -> Color {
  return (color == Color::kWhite) ? Color::kBlack : Color::kWhite;
}

/// @}

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_COLOR_H_
