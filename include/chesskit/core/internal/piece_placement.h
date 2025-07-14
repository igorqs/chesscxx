#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PIECE_PLACEMENT_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PIECE_PLACEMENT_H_

#include <algorithm>
#include <expected>
#include <optional>

#include "../../castling_side.h"
#include "../../color.h"
#include "../../move_error.h"
#include "../../movegen/internal/piece_placement_movegen.h"
#include "../../movegen/internal/square_movegen.h"
#include "../../piece.h"
#include "../../piece_placement.h"
#include "../../piece_type.h"
#include "../../rank.h"
#include "../../square.h"
#include "../../uci_move.h"
#include "castling_rules.h"
#include "piece_placement_piece_at.h"
#include "rank.h"
#include "raw_move.h"
#include "uci_move.h"

namespace chesskit::internal {

inline bool moveResultsInSelfCheck(PiecePlacement, const RawMove&,
                                   const Color&);

inline std::optional<MoveError> castlingError(PiecePlacement,
                                              const CastlingSide&,
                                              const Color&);

inline bool isAttacked(const PiecePlacement& pp, const Square& square,
                       const Color& attackerColor) {
  auto pieces = piecesAttacking(pp, square, attackerColor);
  return pieces.begin() != pieces.end();
}

inline const Square& kingLocation(const PiecePlacement& pp,
                                  const Color& color) {
  return *pp.pieceLocations().at(color).at(PieceType::kKing).begin();
}

inline bool isKingAttacked(const PiecePlacement& pp, const Color& color) {
  return isAttacked(pp, kingLocation(pp, color), !color);
}

inline bool isMoveClear(const PiecePlacement& pp, const RawMove& move) {
  return std::ranges::none_of(
      traversedSquares(move.origin, move.destination),
      [pp](const auto& square) { return hasPieceAt(pp, square); });
}

inline bool isMoveUnderAttack(const PiecePlacement& pp, const RawMove& move,
                              const Color& attackerColor) {
  return std::ranges::any_of(traversedSquares(move.origin, move.destination),
                             [pp, attackerColor](const auto& square) {
                               return isAttacked(pp, square, attackerColor);
                             });
}

inline std::optional<CastlingSide> castlingSideFromUci(const PiecePlacement& pp,
                                                       const UciMove& uci,
                                                       const Color& color) {
  if (uci.promotion) return std::nullopt;

  if (!hasPieceAt(pp, uci.origin, {PieceType::kKing, color})) {
    return std::nullopt;
  }

  if (uci.origin != initialKingSquare(color)) return std::nullopt;

  if (uci.destination == kingsideCastlingKingDestination(color)) {
    return CastlingSide::kKingside;
  }

  if (uci.destination == queensideCastlingKingDestination(color)) {
    return CastlingSide::kQueenside;
  }

  return std::nullopt;
}

inline std::optional<MoveError> promotionError(const Piece& piece,
                                               const Rank& destinationRank) {
  if (piece.type != PieceType::kPawn) {
    return MoveError::kNonPawnPromotionAttempt;
  }

  if (destinationRank != promotionRank(piece.color)) {
    return MoveError::kPromotionOnInvalidRank;
  }

  return std::nullopt;
}

inline std::optional<MoveError> missingPromotionError(
    const Piece& piece, const Rank& destinationRank) {
  if (piece.type == PieceType::kPawn &&
      destinationRank == promotionRank(piece.color)) {
    return MoveError::kMissingPromotionPiece;
  }

  return std::nullopt;
}

inline std::optional<MoveError> normalMoveError(const PiecePlacement& pp,
                                                const UciMove& uci) {
  const auto& origin = uci.origin;
  const auto& destination = uci.destination;

  auto piece = pieceAt(pp, origin);
  if (!piece) return MoveError::kNoPieceAtOrigin;

  if (uci.promotion.has_value()) {
    auto error = promotionError(*piece, destination.rank);
    if (error) return error;
  } else {
    auto error = missingPromotionError(*piece, destination.rank);
    if (error) return error;
  }

  if (moveResultsInSelfCheck(pp, rawMoveFromUci(uci), piece->color)) {
    return MoveError::kMoveLeavesOwnKingInCheck;
  }

  return std::nullopt;
}

class PiecePlacementModifier {
 public:
  static std::expected<void, MoveError> doNormalMove(PiecePlacement& pp,
                                                     const UciMove& uci) {
    auto originPiece = pieceAt(pp, uci.origin);
    if (!originPiece) return std::unexpected(MoveError::kNoPieceAtOrigin);
    const auto color = originPiece->color;

    auto error = normalMoveError(pp, uci);
    if (error.has_value()) return std::unexpected(error.value());

    relocatePiece(pp, uci.origin, uci.destination);

    if (uci.promotion) {
      pp.updatePieceAt(uci.destination,
                       Piece(toPieceType(*uci.promotion), color));
    }

    return {};
  }

  static std::expected<void, MoveError> doCastling(PiecePlacement& pp,
                                                   const CastlingSide& side,
                                                   const Color& color) {
    auto error = castlingError(pp, side, color);
    if (error.has_value()) return std::unexpected(error.value());

    auto moves = castlingMoves(side, color);

    relocatePiece(pp, moves.kingMove.origin, moves.kingMove.destination);
    relocatePiece(pp, moves.rookMove.origin, moves.rookMove.destination);

    return {};
  }

  static void setPieceAt(PiecePlacement& pp, const Square& square,
                         const std::optional<Piece>& piece) {
    pp.updatePieceAt(square, piece);
  }

  static void relocatePiece(PiecePlacement& pp, const Square& origin,
                            const Square& destination) {
    pp.updatePieceAt(destination, pieceAt(pp, origin));
    pp.updatePieceAt(origin, std::nullopt);
  }
};

inline bool moveResultsInSelfCheck(PiecePlacement pp, const RawMove& move,
                                   const Color& color) {
  PiecePlacementModifier::relocatePiece(pp, move.origin, move.destination);
  return isKingAttacked(pp, color);
}

inline std::optional<MoveError> castlingError(PiecePlacement pp,
                                              const CastlingSide& side,
                                              const Color& color) {
  auto moves = castlingMoves(side, color);

  PiecePlacementModifier::setPieceAt(pp, moves.kingMove.origin, std::nullopt);
  PiecePlacementModifier::setPieceAt(pp, moves.rookMove.origin, std::nullopt);

  if (!isMoveClear(pp, moves.kingMove)) {
    return MoveError::kKingPathBlocked;
  }

  if (!isMoveClear(pp, moves.rookMove)) {
    return MoveError::kRookPathBlocked;
  }

  if (isMoveUnderAttack(pp, moves.kingMove, !color)) {
    return MoveError::kKingPathUnderAttack;
  }

  return {};
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PIECE_PLACEMENT_H_
