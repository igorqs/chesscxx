#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_UCI_MOVE_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_UCI_MOVE_H_

#include "../../uci_move.h"
#include "raw_move.h"

namespace chesskit::internal {

inline constexpr RawMove rawMoveFromUci(const UciMove& uci) {
  return RawMove(uci.origin, uci.destination);
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_UCI_MOVE_H_
