#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_CASTLING_SIDE_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_CASTLING_SIDE_H_

// IWYU pragma: private, include "../castling_side.h"

#include <cstdint>

namespace chesscxx {

/// @brief Represents the side of the chessboard for castling.
enum class CastlingSide : uint8_t {
  kKingside,
  kQueenside,
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_CASTLING_SIDE_H_
