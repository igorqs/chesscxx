#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_RAW_MOVE_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_RAW_MOVE_H_

#include "../../square.h"

namespace chesscxx::internal {

struct RawMove {
  Square origin;
  Square destination;
};

constexpr auto reverse(const RawMove& move) -> RawMove {
  return {.origin = move.destination, .destination = move.origin};
}

}  // namespace chesscxx::internal

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_RAW_MOVE_H_
