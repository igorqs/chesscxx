#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_RAW_MOVE_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_RAW_MOVE_H_

#include "../../square.h"

namespace chesskit::internal {

struct RawMove {
  Square origin;
  Square destination;
};

constexpr auto reverse(const RawMove& move) -> RawMove {
  return {.origin = move.destination, .destination = move.origin};
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_RAW_MOVE_H_
