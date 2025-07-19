#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PARTIAL_SQUARE_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PARTIAL_SQUARE_H_

#include <optional>

#include "../../partial_square.h"
#include "../../square.h"

namespace chesskit::internal {

inline auto matches(const PartialSquare& partial, const Square& square)
    -> bool {
  bool const valid_file = (!partial.file) || (*(partial.file) == square.file);
  bool const valid_rank = (!partial.rank) || (*(partial.rank) == square.rank);

  return (valid_file && valid_rank);
}

inline auto toSquare(const PartialSquare& partial) -> std::optional<Square> {
  return (partial.file && partial.rank)
             ? std::optional(Square(*partial.file, *partial.rank))
             : std::nullopt;
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PARTIAL_SQUARE_H_
