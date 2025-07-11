#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_CASTLING_SIDE_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_CASTLING_SIDE_H_

// IWYU pragma: private, include "../castling_side.h"

#include <cstdint>

namespace chesskit {

/// @brief Represents the side of the chessboard for castling.
enum class CastlingSide : uint8_t {
  kKingside,
  kQueenside,
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_CASTLING_SIDE_H_
