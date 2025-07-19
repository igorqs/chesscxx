#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_PIECE_PLACEMENT_IMPL_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_PIECE_PLACEMENT_IMPL_H_

// IWYU pragma: private, include "../piece_placement.h"

#include "../parser/parse.h"
#include "../parser/piece_placement_parser.h"  // IWYU pragma: keep
#include "piece_placement.h"

namespace chesskit {

namespace internal {
constexpr auto standardStartPosition() -> PiecePlacement {
  return *parse<PiecePlacement>("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
}
}  // namespace internal

constexpr PiecePlacement::PiecePlacement()
    : PiecePlacement(internal::standardStartPosition()) {}

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_PIECE_PLACEMENT_IMPL_H_
