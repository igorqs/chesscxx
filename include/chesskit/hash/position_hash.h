#ifndef CHESSKIT_INCLUDE_CHESSKIT_HASH_POSITION_HASH_H_
#define CHESSKIT_INCLUDE_CHESSKIT_HASH_POSITION_HASH_H_

// IWYU pragma: private, include "../position.h"

#include <cstddef>
#include <functional>

#include "../core/internal/square.h"
#include "../core/position.h"
#include "../core/position_impl.h"  // IWYU pragma: keep
#include "internal/hash_combine.h"

namespace chesskit {

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
}  // namespace chesskit

/// @ingroup PositionHelpers
/// @brief default hash support for chesskit::Position
template <>
struct std::hash<chesskit::Position> {
  auto operator()(const chesskit::Position& position) const -> size_t {
    return chesskit::internal::hashCombine(
        position.piecePlacement(), position.activeColor(),
        chesskit::internal::toOptionalFile(position.enPassantTargetSquare()),
        position.halfmoveClock(), position.fullmoveNumber(),
        position.castlingRights());
  };
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_HASH_POSITION_HASH_H_
