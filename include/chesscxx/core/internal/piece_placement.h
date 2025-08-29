#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_PIECE_PLACEMENT_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_PIECE_PLACEMENT_H_

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

namespace chesscxx::internal {

inline auto moveResultsInSelfCheck(PiecePlacement /*piece_placement*/,
                                   const RawMove& /*move*/,
                                   const Color& /*color*/) -> bool;

inline auto enPassantCaptureResultsInSelfCheck(
    PiecePlacement /*piece_placement*/, const RawMove& /*move*/,
    const Square& /*captured_pawn_square*/, const Color& /*color*/) -> bool;

inline auto castlingError(PiecePlacement /*piece_placement*/,
                          const CastlingSide& /*side*/, const Color& /*color*/)
    -> std::optional<MoveError>;

inline auto isAttacked(const PiecePlacement& piece_placement,
                       const Square& square, const Color& attacker_color)
    -> bool {
  auto pieces = piecesAttacking(piece_placement, square, attacker_color);
  return pieces.begin() != pieces.end();
}

inline auto kingLocation(const PiecePlacement& piece_placement,
                         const Color& color) -> const Square& {
  return *piece_placement.pieceLocations()
              .at(color)
              .at(PieceType::kKing)
              .begin();
}

inline auto isKingAttacked(const PiecePlacement& piece_placement,
                           const Color& color) -> bool {
  return isAttacked(piece_placement, kingLocation(piece_placement, color),
                    !color);
}

inline auto isMoveClear(const PiecePlacement& piece_placement,
                        const RawMove& move) -> bool {
  return std::ranges::none_of(traversedSquares(move.origin, move.destination),
                              [piece_placement](const auto& square) {
                                return hasPieceAt(piece_placement, square);
                              });
}

inline auto isMoveUnderAttack(const PiecePlacement& piece_placement,
                              const RawMove& move, const Color& attacker_color)
    -> bool {
  return std::ranges::any_of(
      traversedSquares(move.origin, move.destination),
      [piece_placement, attacker_color](const auto& square) {
        return isAttacked(piece_placement, square, attacker_color);
      });
}

inline auto castlingSideFromUci(const PiecePlacement& piece_placement,
                                const UciMove& uci, const Color& color)
    -> std::optional<CastlingSide> {
  if (uci.promotion) return std::nullopt;

  if (!hasPieceAt(piece_placement, uci.origin,
                  {.type = PieceType::kKing, .color = color})) {
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

inline auto promotionError(const Piece& piece, const Rank& destination_rank)
    -> std::optional<MoveError> {
  if (piece.type != PieceType::kPawn) {
    return MoveError::kNonPawnPromotionAttempt;
  }

  if (destination_rank != promotionRank(piece.color)) {
    return MoveError::kPromotionOnInvalidRank;
  }

  return std::nullopt;
}

inline auto missingPromotionError(const Piece& piece,
                                  const Rank& destination_rank)
    -> std::optional<MoveError> {
  if (piece.type == PieceType::kPawn &&
      destination_rank == promotionRank(piece.color)) {
    return MoveError::kMissingPromotionPiece;
  }

  return std::nullopt;
}

inline auto normalMoveError(const PiecePlacement& piece_placement,
                            const UciMove& uci) -> std::optional<MoveError> {
  const auto& origin = uci.origin;
  const auto& destination = uci.destination;

  auto piece = pieceAt(piece_placement, origin);
  if (!piece) return MoveError::kNoPieceAtOrigin;

  if (uci.promotion.has_value()) {
    auto error = promotionError(*piece, destination.rank);
    if (error) return error;
  } else {
    auto error = missingPromotionError(*piece, destination.rank);
    if (error) return error;
  }

  if (moveResultsInSelfCheck(piece_placement, rawMoveFromUci(uci),
                             piece->color)) {
    return MoveError::kMoveLeavesOwnKingInCheck;
  }

  return std::nullopt;
}

inline auto enPassantCaptureError(const PiecePlacement& piece_placement,
                                  const UciMove& uci,
                                  const Square& captured_pawn_square)
    -> std::optional<MoveError> {
  const auto& origin = uci.origin;
  const auto& destination = uci.destination;

  auto piece = pieceAt(piece_placement, origin);
  if (!piece) return MoveError::kNoPieceAtOrigin;

  if (uci.promotion.has_value()) {
    auto error = promotionError(*piece, destination.rank);
    if (error) return error;
  } else {
    auto error = missingPromotionError(*piece, destination.rank);
    if (error) return error;
  }

  if (enPassantCaptureResultsInSelfCheck(piece_placement, rawMoveFromUci(uci),
                                         captured_pawn_square, piece->color)) {
    return MoveError::kMoveLeavesOwnKingInCheck;
  }

  return std::nullopt;
}

class PiecePlacementModifier {
 public:
  static auto doNormalMove(PiecePlacement& piece_placement, const UciMove& uci)
      -> std::expected<void, MoveError> {
    auto origin_piece = pieceAt(piece_placement, uci.origin);
    if (!origin_piece) return std::unexpected(MoveError::kNoPieceAtOrigin);
    const auto color = origin_piece->color;

    auto error = normalMoveError(piece_placement, uci);
    if (error.has_value()) return std::unexpected(error.value());

    relocatePiece(piece_placement, rawMoveFromUci(uci));

    if (uci.promotion) {
      piece_placement.updatePieceAt(uci.destination,
                                    Piece(toPieceType(*uci.promotion), color));
    }

    return {};
  }

  static auto doEnPassantCapture(PiecePlacement& piece_placement,
                                 const UciMove& uci,
                                 const Square& captured_pawn_square)
      -> std::expected<void, MoveError> {
    auto error =
        enPassantCaptureError(piece_placement, uci, captured_pawn_square);
    if (error.has_value()) return std::unexpected(error.value());

    relocatePiece(piece_placement, rawMoveFromUci(uci));
    setPieceAt(piece_placement, captured_pawn_square, std::nullopt);

    return {};
  }

  static auto doCastling(PiecePlacement& piece_placement,
                         const CastlingSide& side, const Color& color)
      -> std::expected<void, MoveError> {
    auto error = castlingError(piece_placement, side, color);
    if (error.has_value()) return std::unexpected(error.value());

    auto moves = castlingMoves(side, color);

    relocatePiece(piece_placement, moves.king_move);
    relocatePiece(piece_placement, moves.rook_move);

    return {};
  }

  static void setPieceAt(PiecePlacement& piece_placement, const Square& square,
                         const std::optional<Piece>& piece) {
    piece_placement.updatePieceAt(square, piece);
  }

  static void relocatePiece(PiecePlacement& piece_placement,
                            const RawMove& move) {
    piece_placement.updatePieceAt(move.destination,
                                  pieceAt(piece_placement, move.origin));
    piece_placement.updatePieceAt(move.origin, std::nullopt);
  }
};

inline auto moveResultsInSelfCheck(PiecePlacement piece_placement,
                                   const RawMove& move, const Color& color)
    -> bool {
  PiecePlacementModifier::relocatePiece(piece_placement, move);
  return isKingAttacked(piece_placement, color);
}

inline auto enPassantCaptureResultsInSelfCheck(
    PiecePlacement piece_placement, const RawMove& move,
    const Square& captured_pawn_square, const Color& color) -> bool {
  PiecePlacementModifier::relocatePiece(piece_placement, move);
  PiecePlacementModifier::setPieceAt(piece_placement, captured_pawn_square,
                                     std::nullopt);
  return isKingAttacked(piece_placement, color);
}

inline auto castlingError(PiecePlacement piece_placement,
                          const CastlingSide& side, const Color& color)
    -> std::optional<MoveError> {
  auto moves = castlingMoves(side, color);

  PiecePlacementModifier::setPieceAt(piece_placement, moves.king_move.origin,
                                     std::nullopt);
  PiecePlacementModifier::setPieceAt(piece_placement, moves.rook_move.origin,
                                     std::nullopt);

  if (!isMoveClear(piece_placement, moves.king_move)) {
    return MoveError::kKingPathBlocked;
  }

  if (!isMoveClear(piece_placement, moves.rook_move)) {
    return MoveError::kRookPathBlocked;
  }

  if (isMoveUnderAttack(piece_placement, moves.king_move, !color)) {
    return MoveError::kKingPathUnderAttack;
  }

  return {};
}

}  // namespace chesscxx::internal

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_PIECE_PLACEMENT_H_
