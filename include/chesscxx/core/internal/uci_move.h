#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_UCI_MOVE_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_UCI_MOVE_H_

#include "../../uci_move.h"
#include "raw_move.h"

namespace chesscxx::internal {

constexpr auto rawMoveFromUci(const UciMove& uci) -> RawMove {
  return RawMove(uci.origin, uci.destination);
}

}  // namespace chesscxx::internal

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_UCI_MOVE_H_
