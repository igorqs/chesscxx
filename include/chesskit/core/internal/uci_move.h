#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_UCI_MOVE_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_UCI_MOVE_H_

#include "../../uci_move.h"
#include "raw_move.h"

namespace chesskit::internal {

constexpr auto rawMoveFromUci(const UciMove& uci) -> RawMove {
  return RawMove(uci.origin, uci.destination);
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_UCI_MOVE_H_
