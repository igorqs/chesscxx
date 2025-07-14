#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_POSITION_MODIFIER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_POSITION_MODIFIER_H_

#include <expected>
#include <optional>
#include <variant>

#include "../../castling_side.h"
#include "../../check_indicator.h"
#include "../../color.h"
#include "../../move_error.h"
#include "../../piece_type.h"
#include "../../position.h"
#include "../../san_move.h"
#include "../../uci_move.h"
#include "castling_rules.h"
#include "move_record.h"
#include "piece_placement.h"
#include "piece_placement_piece_at.h"
#include "position.h"
#include "rank.h"
#include "raw_move.h"
#include "square.h"
#include "uci_move.h"

namespace chesskit::internal {

class PositionModifier {
 public:
  static void resetMoveCounters(Position& position) {
    position.halfmoveClock_ = Position::kMinHalfmoveClock;
    position.fullmoveNumber_ = Position::kMinFullmoveNumber;
  }

  template <typename MoveNotation>
  static std::expected<MoveRecord, MoveError> move(Position& position,
                                                   const MoveNotation& move) {
    auto result = executeMove(position, move);

    if (result) {
      position.toggleActiveColor();

      if (isCheckmate(position)) {
        std::visit(
            [](auto& r) { r.check_indicator = CheckIndicator::kCheckmate; },
            *result);
      } else if (isCheck(position)) {
        std::visit([](auto& r) { r.check_indicator = CheckIndicator::kCheck; },
                   *result);
      }
    }

    return result;
  }

  static void undoMove(Position& position, const MoveRecord& move) {
    std::visit([&](const auto& c) { undoMove(position, c); }, move);
  }

 private:
  static std::expected<MoveRecord, MoveError> executeMove(Position& position,
                                                          const SanMove& move) {
    return std::visit(
        [&](const auto& arg) { return executeMove(position, arg); }, move);
  }

  static std::expected<MoveRecord, MoveError> executeMove(
      Position& position, const SanNormalMove& move) {
    auto uci = uciFromSan(position, move, position.activeColor());

    if (!uci) return std::unexpected(uci.error());

    return executeNormalMove(position, *uci);
  }

  static std::expected<MoveRecord, MoveError> executeMove(
      Position& position, const SanCastlingMove& move) {
    return executeCastling(position, move.side);
  }

  static std::expected<MoveRecord, MoveError> executeMove(Position& position,
                                                          const UciMove& move) {
    auto castlingSide = castlingSideFromUci(position.piecePlacement(), move,
                                            position.activeColor());

    if (castlingSide) return executeCastling(position, *castlingSide);

    if (auto err = uciMoveError(position, rawMoveFromUci(move))) {
      return std::unexpected(*err);
    }

    return executeNormalMove(position, move);
  }

  static std::expected<MoveRecord, MoveError> executeNormalMove(
      Position& position, const UciMove& uci) {
    const auto& castlingRights = position.castlingRights();
    const auto& activeColor = position.activeColor();
    const auto& enPassantFile = position.enPassantFile_;
    const auto& halfmoveClock = position.halfmoveClock();

    if (auto error = overflowError(position, rawMoveFromUci(uci))) {
      return std::unexpected(error.value());
    }

    auto destinationPiece = pieceAt(position.piecePlacement_, uci.destination);

    auto originPiece = pieceAt(position.piecePlacement_, uci.origin);
    if (!originPiece) return std::unexpected(MoveError::kNoPieceAtOrigin);
    if (originPiece->color != activeColor) {
      return std::unexpected(MoveError::kWrongPieceColorAtOrigin);
    }

    bool isPawnMove = originPiece->type == PieceType::kPawn;
    bool isNormalCapture = destinationPiece.has_value();
    auto partialOrigin = partialOriginFromMove(position, rawMoveFromUci(uci));

    auto result =
        PiecePlacementModifier::doNormalMove(position.piecePlacement_, uci);

    if (result) {
      auto previousCastlingRights = castlingRights;

      NormalMoveRecord moveRecord = {
          .pieceType = originPiece->type,
          .partialOrigin = *partialOrigin,
          .uciMove = uci,
          .previousEnPassantFile = enPassantFile,
          .previousHalfmoveClock = halfmoveClock,
      };

      if (isNormalCapture) {
        moveRecord.capturedPieceType = destinationPiece->type;
      }

      // Handle en passant capture
      bool isEnPassantCapture =
          isPawnMove && position.enPassantTargetSquare() == uci.destination;
      if (isEnPassantCapture) {
        if (auto capturedPawnSquare =
                enPassantCapturedPawnSquare(uci.destination, activeColor)) {
          moveRecord.isEnPassantCapture = true;
          PiecePlacementModifier::setPieceAt(position.piecePlacement_,
                                             *capturedPawnSquare, std::nullopt);
        }
      }

      // Update castling rights
      updateCastlingRights(position, rawMoveFromUci(uci));

      // Update en passant
      if (isPawnMove) {
        updateEnPassantTargetForPawnMove(position, rawMoveFromUci(uci));
      } else {
        position.resetEnPassantFile();
      }

      // Update move counters
      position.incrementFullmoveNumber();
      if (isPawnMove || isNormalCapture) {
        position.resetHalfmoveClock();
      } else {
        position.incrementHalfmoveClock();
      }

      if (previousCastlingRights != castlingRights) {
        moveRecord.previousCastlingRights = previousCastlingRights;
      }

      return moveRecord;
    }

    return std::unexpected(result.error());
  }

  static void updateCastlingRights(Position& position, const RawMove& move) {
    for (auto color : {Color::kBlack, Color::kWhite}) {
      if (affectsKingsideCastling(move, color)) {
        position.castlingRights_.disable(CastlingSide::kKingside, color);
      }
      if (affectsQueensideCastling(move, color)) {
        position.castlingRights_.disable(CastlingSide::kQueenside, color);
      }
    }
  }

  static void updateEnPassantTargetForPawnMove(Position& position,
                                               const RawMove& move) {
    position.resetEnPassantFile();

    const auto& activeColor = position.activeColor();
    const auto& pp = position.piecePlacement();
    const auto& origin = move.origin;
    const auto& dest = move.destination;

    bool isDoublePush = isPawnStartingRank(origin.rank, activeColor) &&
                        isDoublePawnPushTargetRank(dest.rank, activeColor) &&
                        origin.file == dest.file;

    if (!isDoublePush) return;

    position.enPassantFile_ = dest.file;
  }

  static std::expected<MoveRecord, MoveError> executeCastling(
      Position& position, const CastlingSide& side) {
    const auto& castlingRights = position.castlingRights();
    const auto& activeColor = position.activeColor();
    const auto& enPassantFile = position.enPassantFile_;

    if (auto error = overflowError(position)) {
      return std::unexpected(error.value());
    }

    if (!castlingRights.canCastle(side, activeColor)) {
      return std::unexpected(MoveError::kKingOrRookMoved);
    }

    auto result = PiecePlacementModifier::doCastling(position.piecePlacement_,
                                                     side, activeColor);

    if (result) {
      CastlingMoveRecord move = {
          .side = side,
          .color = activeColor,
          .previousCastlingRights = castlingRights,
          .previousEnPassantFile = enPassantFile,
      };

      position.castlingRights_.disable(activeColor);
      position.resetEnPassantFile();
      position.incrementMoveCounters();

      return move;
    }

    return std::unexpected(result.error());
  }

  template <typename T>
  static void undoCommonMoveEffects(Position& position, const T& move) {
    position.enPassantFile_ = move.previousEnPassantFile;
    if (position.activeColor() == Color::kWhite) position.fullmoveNumber_ -= 1;

    position.toggleActiveColor();
  }

  static void undoMove(Position& position, const NormalMoveRecord& move) {
    undoNormalMove(position, move);
    position.halfmoveClock_ = move.previousHalfmoveClock;
    if (move.previousCastlingRights) {
      position.castlingRights_ = *move.previousCastlingRights;
    }

    undoCommonMoveEffects(position, move);
  }

  static void undoMove(Position& position, const CastlingMoveRecord& move) {
    undoCastling(position, move.side);
    position.halfmoveClock_ -= 1;
    position.castlingRights_ = move.previousCastlingRights;

    undoCommonMoveEffects(position, move);
  }

  static void undoNormalMove(Position& position, const NormalMoveRecord& move) {
    const auto& origin = move.uciMove.origin;
    const auto& destination = move.uciMove.destination;
    const auto& capturedPieceType = move.capturedPieceType;
    const auto mover_color = !position.activeColor();
    const auto opponent_color = !mover_color;
    auto& pp = position.piecePlacement_;

    bool wasPromotion = move.pieceType == PieceType::kPawn &&
                        destination.rank == promotionRank(mover_color);

    if (wasPromotion) {
      auto piece = Piece(PieceType::kPawn, mover_color);
      PiecePlacementModifier::setPieceAt(pp, destination, piece);
    }

    PiecePlacementModifier::relocatePiece(pp, destination, origin);

    if (capturedPieceType) {
      auto capturedPiece = Piece(*capturedPieceType, opponent_color);
      PiecePlacementModifier::setPieceAt(pp, destination, capturedPiece);
    }

    if (move.isEnPassantCapture) {
      if (auto capturedPawnSquare =
              enPassantCapturedPawnSquare(destination, mover_color)) {
        auto capturedPiece = Piece(PieceType::kPawn, opponent_color);
        PiecePlacementModifier::setPieceAt(pp, *capturedPawnSquare,
                                           capturedPiece);
      }
    }
  }

  static void undoCastling(Position& position, const CastlingSide& side) {
    const auto previousColor = !position.activeColor();
    auto& pp = position.piecePlacement_;

    auto kingOrigin = initialKingSquare(previousColor);
    auto kingDestination = kingsideCastlingKingDestination(previousColor);
    auto rookOrigin = initialKingsideRookSquare(previousColor);
    auto rookDestination = kingsideCastlingRookDestination(previousColor);

    if (side == CastlingSide::kQueenside) {
      kingDestination = queensideCastlingKingDestination(previousColor);
      rookOrigin = initialQueensideRookSquare(previousColor);
      rookDestination = queensideCastlingRookDestination(previousColor);
    }

    PiecePlacementModifier::relocatePiece(pp, kingDestination, kingOrigin);
    PiecePlacementModifier::relocatePiece(pp, rookDestination, rookOrigin);
  }
};

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_POSITION_MODIFIER_H_
