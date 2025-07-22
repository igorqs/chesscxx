#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PIECE_PLACEMENT_PIECE_AT_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PIECE_PLACEMENT_PIECE_AT_H_

#include <optional>

#include "../../color.h"
#include "../../piece.h"
#include "../../piece_placement.h"
#include "../../piece_type.h"
#include "../../square.h"

namespace chesskit::internal {

inline auto pieceAt(const PiecePlacement& piece_placement, const Square& square)
    -> const std::optional<Piece>& {
  return piece_placement.pieceArray().at(index(square));
}

inline auto hasPieceAt(const PiecePlacement& piece_placement,
                       const Square& square) -> bool {
  return pieceAt(piece_placement, square).has_value();
}

inline auto hasPieceAt(const PiecePlacement& piece_placement,
                       const Square& square, const PieceType& type) -> bool {
  const auto& piece = pieceAt(piece_placement, square);
  return piece && piece->type == type;
}

inline auto hasPieceAt(const PiecePlacement& piece_placement,
                       const Square& square, const Color& color) -> bool {
  const auto& piece = pieceAt(piece_placement, square);
  return piece && piece->color == color;
}

inline auto hasPieceAt(const PiecePlacement& piece_placement,
                       const Square& square, const Piece& piece) -> bool {
  const auto& square_piece = pieceAt(piece_placement, square);
  return square_piece && *square_piece == piece;
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PIECE_PLACEMENT_PIECE_AT_H_
