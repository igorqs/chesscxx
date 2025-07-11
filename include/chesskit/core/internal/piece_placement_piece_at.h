#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PIECE_PLACEMENT_PIECE_AT_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PIECE_PLACEMENT_PIECE_AT_H_

#include <optional>

#include "../../color.h"
#include "../../piece.h"
#include "../../piece_placement.h"
#include "../../piece_type.h"
#include "../../square.h"

namespace chesskit::internal {

inline const std::optional<Piece>& pieceAt(const PiecePlacement& pp,
                                           const Square& square) {
  return pp.pieceArray()[index(square)];
}

inline bool hasPieceAt(const PiecePlacement& pp, const Square& square) {
  return pieceAt(pp, square).has_value();
}

inline bool hasPieceAt(const PiecePlacement& pp, const Square& square,
                       const PieceType& type) {
  const auto& piece = pieceAt(pp, square);
  return piece && piece->type == type;
}

inline bool hasPieceAt(const PiecePlacement& pp, const Square& square,
                       const Color& color) {
  const auto& piece = pieceAt(pp, square);
  return piece && piece->color == color;
}

inline bool hasPieceAt(const PiecePlacement& pp, const Square& square,
                       const Piece& piece) {
  const auto& squarePiece = pieceAt(pp, square);
  return squarePiece && *squarePiece == piece;
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PIECE_PLACEMENT_PIECE_AT_H_
