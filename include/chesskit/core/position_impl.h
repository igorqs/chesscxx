#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_POSITION_IMPL_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_POSITION_IMPL_H_

// IWYU pragma: private, include "../position.h"

#include <optional>

#include "../movegen/internal/position_en_passant_movegen.h"
#include "../square.h"
#include "position.h"

namespace chesskit {

inline std::optional<Square> Position::legalEnPassantTargetSquare() const {
  if (internal::hasLegalEnPassantCapture(*this)) return enPassantTargetSquare();

  return std::nullopt;
}

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_POSITION_IMPL_H_
