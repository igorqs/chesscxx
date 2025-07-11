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
  RawMove kingMove;
  RawMove rookMove;
};

inline constexpr Square initialKingSquare(const Color& color) {
  return backRankSquare(File::kE, color);
}

inline constexpr Square kingsideCastlingKingDestination(const Color& color) {
  return backRankSquare(File::kG, color);
}

inline constexpr Square initialKingsideRookSquare(const Color& color) {
  return backRankSquare(File::kH, color);
}

inline constexpr Square kingsideCastlingRookDestination(const Color& color) {
  return backRankSquare(File::kF, color);
}

inline constexpr Square queensideCastlingKingDestination(const Color& color) {
  return backRankSquare(File::kC, color);
}

inline constexpr Square initialQueensideRookSquare(const Color& color) {
  return backRankSquare(File::kA, color);
}

inline constexpr Square queensideCastlingRookDestination(const Color& color) {
  return backRankSquare(File::kD, color);
}

inline constexpr CastlingMoves kingsideCastlingMoves(const Color& color) {
  RawMove kingMove =
      RawMove(initialKingSquare(color), kingsideCastlingKingDestination(color));
  RawMove rookMove = RawMove(initialKingsideRookSquare(color),
                             kingsideCastlingRookDestination(color));

  return CastlingMoves(kingMove, rookMove);
}

inline constexpr CastlingMoves queensideCastlingMoves(const Color& color) {
  RawMove kingMove = RawMove(initialKingSquare(color),
                             queensideCastlingKingDestination(color));
  RawMove rookMove = RawMove(initialQueensideRookSquare(color),
                             queensideCastlingRookDestination(color));

  return CastlingMoves(kingMove, rookMove);
}

inline constexpr CastlingMoves castlingMoves(const CastlingSide& side,
                                             const Color& color) {
  return side == CastlingSide::kKingside ? kingsideCastlingMoves(color)
                                         : queensideCastlingMoves(color);
}

inline constexpr bool affectsKingsideCastling(const RawMove& move,
                                              const Color& color) {
  return (move.origin == initialKingSquare(color)) ||
         (move.origin == initialKingsideRookSquare(color)) ||
         (move.destination == initialKingsideRookSquare(color));
}

inline constexpr bool affectsQueensideCastling(const RawMove& move,
                                               const Color& color) {
  return (move.origin == initialKingSquare(color)) ||
         (move.origin == initialQueensideRookSquare(color)) ||
         (move.destination == initialQueensideRookSquare(color));
}

inline constexpr bool areCastlingPiecesInInitialLocation(
    const PiecePlacement& pp, const CastlingSide& side, const Color& color) {
  const auto king = Piece(PieceType::kKing, color);
  const auto rook = Piece(PieceType::kRook, color);
  const auto rookSquare = (side == CastlingSide::kKingside)
                              ? initialKingsideRookSquare(color)
                              : initialQueensideRookSquare(color);

  if (!hasPieceAt(pp, initialKingSquare(color), king)) return false;
  if (!hasPieceAt(pp, rookSquare, rook)) return false;

  return true;
}

inline constexpr bool isValidCastlingRights(const PiecePlacement& pp,
                                            const CastlingRights& rights) {
  for (const auto& side : {CastlingSide::kKingside, CastlingSide::kQueenside}) {
    for (const auto& color : {Color::kWhite, Color::kBlack}) {
      if (rights.canCastle(side, color) &&
          !areCastlingPiecesInInitialLocation(pp, side, color))
        return false;
    }
  }

  return true;
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_CASTLING_RULES_H_
