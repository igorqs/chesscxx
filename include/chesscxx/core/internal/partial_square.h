#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_PARTIAL_SQUARE_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_PARTIAL_SQUARE_H_

#include <optional>

#include "../../partial_square.h"
#include "../../square.h"

namespace chesscxx::internal {

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

}  // namespace chesscxx::internal

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_PARTIAL_SQUARE_H_
