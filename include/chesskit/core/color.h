#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_COLOR_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_COLOR_H_

// IWYU pragma: private, include "../color.h"

#include <cstdint>

namespace chesskit {
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

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_COLOR_H_
