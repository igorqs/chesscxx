#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PARTIAL_SQUARE_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PARTIAL_SQUARE_H_

#include <optional>

#include "../../partial_square.h"
#include "../../square.h"

namespace chesskit::internal {

inline bool matches(const PartialSquare& partial, const Square& square) {
  bool validFile = (!partial.file) || (*(partial.file) == square.file);
  bool validRank = (!partial.rank) || (*(partial.rank) == square.rank);

  return (validFile && validRank);
}

inline std::optional<Square> toSquare(const PartialSquare& partial) {
  return (partial.file && partial.rank)
             ? std::optional(Square(*partial.file, *partial.rank))
             : std::nullopt;
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PARTIAL_SQUARE_H_
