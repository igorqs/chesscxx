#ifndef CHESSCXX_INCLUDE_CHESSCXX_HASH_POSITION_HASH_H_
#define CHESSCXX_INCLUDE_CHESSCXX_HASH_POSITION_HASH_H_

// IWYU pragma: private, include "../position.h"

#include <cstddef>
#include <functional>

#include "../core/internal/square.h"
#include "../core/position.h"
#include "../core/position_impl.h"  // IWYU pragma: keep
#include "internal/hash_combine.h"

namespace chesscxx {

/// @ingroup PositionHelpers
/// @brief Equality predicate for Position used for repetition detection.
struct RepetitionEqual {
  auto operator()(const Position& lhs, const Position& rhs) const -> bool {
    const auto lhs_en_passant =
        internal::toOptionalFile(lhs.legalEnPassantTargetSquare());
    const auto rhs_en_passant =
        internal::toOptionalFile(rhs.legalEnPassantTargetSquare());

    return lhs.activeColor() == rhs.activeColor() &&
           lhs_en_passant == rhs_en_passant &&
           lhs.castlingRights() == rhs.castlingRights() &&
           lhs.piecePlacement() == rhs.piecePlacement();
  }
};

/// @ingroup PositionHelpers
/// @brief Hash support for Position used for repetition detection.
struct RepetitionHash {
  auto operator()(const Position& position) const -> size_t {
    const auto en_passant =
        internal::toOptionalFile(position.legalEnPassantTargetSquare());

    return internal::hashCombine(position.piecePlacement(),
                                 position.activeColor(), en_passant,
                                 position.castlingRights());
  }
};
}  // namespace chesscxx

/// @ingroup PositionHelpers
/// @brief default hash support for chesscxx::Position
template <>
struct std::hash<chesscxx::Position> {
  auto operator()(const chesscxx::Position& position) const -> size_t {
    return chesscxx::internal::hashCombine(
        position.piecePlacement(), position.activeColor(),
        chesscxx::internal::toOptionalFile(position.enPassantTargetSquare()),
        position.halfmoveClock(), position.fullmoveNumber(),
        position.castlingRights());
  };
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_HASH_POSITION_HASH_H_
