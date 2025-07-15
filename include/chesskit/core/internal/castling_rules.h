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

inline constexpr auto initialKingSquare(const Color& color) -> Square {
  return backRankSquare(File::kE, color);
}

inline constexpr auto kingsideCastlingKingDestination(const Color& color)
    -> Square {
  return backRankSquare(File::kG, color);
}

inline constexpr auto initialKingsideRookSquare(const Color& color) -> Square {
  return backRankSquare(File::kH, color);
}

inline constexpr auto kingsideCastlingRookDestination(const Color& color)
    -> Square {
  return backRankSquare(File::kF, color);
}

inline constexpr auto queensideCastlingKingDestination(const Color& color)
    -> Square {
  return backRankSquare(File::kC, color);
}

inline constexpr auto initialQueensideRookSquare(const Color& color) -> Square {
  return backRankSquare(File::kA, color);
}

inline constexpr auto queensideCastlingRookDestination(const Color& color)
    -> Square {
  return backRankSquare(File::kD, color);
}

inline constexpr auto kingsideCastlingMoves(const Color& color)
    -> CastlingMoves {
  RawMove const kingMove =
      RawMove(initialKingSquare(color), kingsideCastlingKingDestination(color));
  RawMove const rookMove = RawMove(initialKingsideRookSquare(color),
                                   kingsideCastlingRookDestination(color));

  return CastlingMoves(kingMove, rookMove);
}

inline constexpr auto queensideCastlingMoves(const Color& color)
    -> CastlingMoves {
  RawMove const kingMove = RawMove(initialKingSquare(color),
                                   queensideCastlingKingDestination(color));
  RawMove const rookMove = RawMove(initialQueensideRookSquare(color),
                                   queensideCastlingRookDestination(color));

  return CastlingMoves(kingMove, rookMove);
}

inline constexpr auto castlingMoves(const CastlingSide& side,
                                    const Color& color) -> CastlingMoves {
  return side == CastlingSide::kKingside ? kingsideCastlingMoves(color)
                                         : queensideCastlingMoves(color);
}

inline constexpr auto affectsKingsideCastling(const RawMove& move,
                                              const Color& color) -> bool {
  return (move.origin == initialKingSquare(color)) ||
         (move.origin == initialKingsideRookSquare(color)) ||
         (move.destination == initialKingsideRookSquare(color));
}

inline constexpr auto affectsQueensideCastling(const RawMove& move,
                                               const Color& color) -> bool {
  return (move.origin == initialKingSquare(color)) ||
         (move.origin == initialQueensideRookSquare(color)) ||
         (move.destination == initialQueensideRookSquare(color));
}

inline constexpr auto areCastlingPiecesInInitialLocation(
    const PiecePlacement& pp, const CastlingSide& side, const Color& color)
    -> bool {
  const auto king = Piece(PieceType::kKing, color);
  const auto rook = Piece(PieceType::kRook, color);
  const auto rookSquare = (side == CastlingSide::kKingside)
                              ? initialKingsideRookSquare(color)
                              : initialQueensideRookSquare(color);

  if (!hasPieceAt(pp, initialKingSquare(color), king)) return false;
  if (!hasPieceAt(pp, rookSquare, rook)) return false;

  return true;
}

inline constexpr auto isValidCastlingRights(const PiecePlacement& pp,
                                            const CastlingRights& rights)
    -> bool {
  for (const auto& side : {CastlingSide::kKingside, CastlingSide::kQueenside}) {
    for (const auto& color : {Color::kWhite, Color::kBlack}) {
      if (rights.canCastle(side, color) &&
          !areCastlingPiecesInInitialLocation(pp, side, color)) {
        return false;
      }
    }
  }

  return true;
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_CASTLING_RULES_H_
