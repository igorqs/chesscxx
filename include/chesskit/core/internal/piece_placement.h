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

inline auto moveResultsInSelfCheck(PiecePlacement, const RawMove&, const Color&)
    -> bool;

inline auto castlingError(PiecePlacement, const CastlingSide&, const Color&)
    -> std::optional<MoveError>;

inline auto isAttacked(const PiecePlacement& pp, const Square& square,
                       const Color& attackerColor) -> bool {
  auto pieces = piecesAttacking(pp, square, attackerColor);
  return pieces.begin() != pieces.end();
}

inline auto kingLocation(const PiecePlacement& pp, const Color& color)
    -> const Square& {
  return *pp.pieceLocations().at(color).at(PieceType::kKing).begin();
}

inline auto isKingAttacked(const PiecePlacement& pp, const Color& color)
    -> bool {
  return isAttacked(pp, kingLocation(pp, color), !color);
}

inline auto isMoveClear(const PiecePlacement& pp, const RawMove& move) -> bool {
  return std::ranges::none_of(
      traversedSquares(move.origin, move.destination),
      [pp](const auto& square) { return hasPieceAt(pp, square); });
}

inline auto isMoveUnderAttack(const PiecePlacement& pp, const RawMove& move,
                              const Color& attackerColor) -> bool {
  return std::ranges::any_of(traversedSquares(move.origin, move.destination),
                             [pp, attackerColor](const auto& square) {
                               return isAttacked(pp, square, attackerColor);
                             });
}

inline auto castlingSideFromUci(const PiecePlacement& pp, const UciMove& uci,
                                const Color& color)
    -> std::optional<CastlingSide> {
  if (uci.promotion) return std::nullopt;

  if (!hasPieceAt(pp, uci.origin, {.type = PieceType::kKing, .color = color})) {
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

inline auto promotionError(const Piece& piece, const Rank& destinationRank)
    -> std::optional<MoveError> {
  if (piece.type != PieceType::kPawn) {
    return MoveError::kNonPawnPromotionAttempt;
  }

  if (destinationRank != promotionRank(piece.color)) {
    return MoveError::kPromotionOnInvalidRank;
  }

  return std::nullopt;
}

inline auto missingPromotionError(const Piece& piece,
                                  const Rank& destinationRank)
    -> std::optional<MoveError> {
  if (piece.type == PieceType::kPawn &&
      destinationRank == promotionRank(piece.color)) {
    return MoveError::kMissingPromotionPiece;
  }

  return std::nullopt;
}

inline auto normalMoveError(const PiecePlacement& pp, const UciMove& uci)
    -> std::optional<MoveError> {
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
  static auto doNormalMove(PiecePlacement& pp, const UciMove& uci)
      -> std::expected<void, MoveError> {
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

  static auto doCastling(PiecePlacement& pp, const CastlingSide& side,
                         const Color& color) -> std::expected<void, MoveError> {
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

inline auto moveResultsInSelfCheck(PiecePlacement pp, const RawMove& move,
                                   const Color& color) -> bool {
  PiecePlacementModifier::relocatePiece(pp, move.origin, move.destination);
  return isKingAttacked(pp, color);
}

inline auto castlingError(PiecePlacement pp, const CastlingSide& side,
                          const Color& color) -> std::optional<MoveError> {
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
