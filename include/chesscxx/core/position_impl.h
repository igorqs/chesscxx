#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_POSITION_IMPL_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_POSITION_IMPL_H_

// IWYU pragma: private, include "../position.h"

#include <optional>

#include "../movegen/internal/position_en_passant_movegen.h"
#include "../square.h"
#include "position.h"

namespace chesscxx {

inline auto Position::legalEnPassantTargetSquare() const
    -> std::optional<Square> {
  if (internal::hasLegalEnPassantCapture(*this)) return enPassantTargetSquare();

  return std::nullopt;
}

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_POSITION_IMPL_H_
