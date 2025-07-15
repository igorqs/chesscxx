#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PARTIAL_SQUARE_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PARTIAL_SQUARE_H_

#include <optional>

#include "../../partial_square.h"
#include "../../square.h"

namespace chesskit::internal {

inline auto matches(const PartialSquare& partial, const Square& square)
    -> bool {
  bool const validFile = (!partial.file) || (*(partial.file) == square.file);
  bool const validRank = (!partial.rank) || (*(partial.rank) == square.rank);

  return (validFile && validRank);
}

inline auto toSquare(const PartialSquare& partial) -> std::optional<Square> {
  return (partial.file && partial.rank)
             ? std::optional(Square(*partial.file, *partial.rank))
             : std::nullopt;
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PARTIAL_SQUARE_H_
