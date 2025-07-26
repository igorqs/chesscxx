#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_PIECE_PLACEMENT_IMPL_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_PIECE_PLACEMENT_IMPL_H_

// IWYU pragma: private, include "../piece_placement.h"

#include "../parser/parse.h"
#include "../parser/piece_placement_parser.h"  // IWYU pragma: keep
#include "piece_placement.h"

namespace chesscxx {

namespace internal {
constexpr auto standardStartPosition() -> PiecePlacement {
  return *parse<PiecePlacement>("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
}
}  // namespace internal

constexpr PiecePlacement::PiecePlacement()
    : PiecePlacement(internal::standardStartPosition()) {}

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_PIECE_PLACEMENT_IMPL_H_
