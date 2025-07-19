#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_CASTLING_RULES_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_CASTLING_RULES_H_

#include "../../castling_rights.h"
#include "../../castling_side.h"
#include "../../color.h"
#include "../../file.h"
#include "../../piece.h"
#include "../../piece_placement.h"
#include "../../piece_type.h"
#include "../../square.h"
#include "piece_placement_piece_at.h"
#include "raw_move.h"
#include "square.h"

namespace chesskit::internal {

struct CastlingMoves {
  RawMove king_move;
  RawMove rook_move;
};

constexpr auto initialKingSquare(const Color& color) -> Square {
  return backRankSquare(File::kE, color);
}

constexpr auto kingsideCastlingKingDestination(const Color& color) -> Square {
  return backRankSquare(File::kG, color);
}

constexpr auto initialKingsideRookSquare(const Color& color) -> Square {
  return backRankSquare(File::kH, color);
}

constexpr auto kingsideCastlingRookDestination(const Color& color) -> Square {
  return backRankSquare(File::kF, color);
}

constexpr auto queensideCastlingKingDestination(const Color& color) -> Square {
  return backRankSquare(File::kC, color);
}

constexpr auto initialQueensideRookSquare(const Color& color) -> Square {
  return backRankSquare(File::kA, color);
}

constexpr auto queensideCastlingRookDestination(const Color& color) -> Square {
  return backRankSquare(File::kD, color);
}

constexpr auto kingsideCastlingMoves(const Color& color) -> CastlingMoves {
  RawMove const king_move =
      RawMove(initialKingSquare(color), kingsideCastlingKingDestination(color));
  RawMove const rook_move = RawMove(initialKingsideRookSquare(color),
                                    kingsideCastlingRookDestination(color));

  return CastlingMoves(king_move, rook_move);
}

constexpr auto queensideCastlingMoves(const Color& color) -> CastlingMoves {
  RawMove const king_move = RawMove(initialKingSquare(color),
                                    queensideCastlingKingDestination(color));
  RawMove const rook_move = RawMove(initialQueensideRookSquare(color),
                                    queensideCastlingRookDestination(color));

  return CastlingMoves(king_move, rook_move);
}

constexpr auto castlingMoves(const CastlingSide& side, const Color& color)
    -> CastlingMoves {
  return side == CastlingSide::kKingside ? kingsideCastlingMoves(color)
                                         : queensideCastlingMoves(color);
}

constexpr auto affectsKingsideCastling(const RawMove& move, const Color& color)
    -> bool {
  return (move.origin == initialKingSquare(color)) ||
         (move.origin == initialKingsideRookSquare(color)) ||
         (move.destination == initialKingsideRookSquare(color));
}

constexpr auto affectsQueensideCastling(const RawMove& move, const Color& color)
    -> bool {
  return (move.origin == initialKingSquare(color)) ||
         (move.origin == initialQueensideRookSquare(color)) ||
         (move.destination == initialQueensideRookSquare(color));
}

constexpr auto areCastlingPiecesInInitialLocation(
    const PiecePlacement& piece_placement, const CastlingSide& side,
    const Color& color) -> bool {
  const auto king = Piece(PieceType::kKing, color);
  const auto rook = Piece(PieceType::kRook, color);
  const auto rook_square = (side == CastlingSide::kKingside)
                               ? initialKingsideRookSquare(color)
                               : initialQueensideRookSquare(color);

  if (!hasPieceAt(piece_placement, initialKingSquare(color), king)) {
    return false;
  }
  if (!hasPieceAt(piece_placement, rook_square, rook)) return false;

  return true;
}

constexpr auto isValidCastlingRights(const PiecePlacement& piece_placement,
                                     const CastlingRights& rights) -> bool {
  for (const auto& side : {CastlingSide::kKingside, CastlingSide::kQueenside}) {
    for (const auto& color : {Color::kWhite, Color::kBlack}) {
      if (rights.canCastle(side, color) &&
          !areCastlingPiecesInInitialLocation(piece_placement, side, color)) {
        return false;
      }
    }
  }

  return true;
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_CASTLING_RULES_H_
