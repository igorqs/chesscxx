#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_POSITION_MODIFIER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_POSITION_MODIFIER_H_

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

namespace chesscxx::internal {

class PositionModifier {
 public:
  static void resetMoveCounters(Position& position) {
    position.halfmove_clock_ = Position::kMinHalfmoveClock;
    position.fullmove_number_ = Position::kMinFullmoveNumber;
  }

  template <typename MoveNotation>
  static auto move(Position& position, const MoveNotation& move)
      -> std::expected<MoveRecord, MoveError> {
    auto result = executeMove(position, move);

    if (result) {
      position.toggleActiveColor();

      if (isCheckmate(position)) {
        std::visit(
            [](auto& arg) { arg.check_indicator = CheckIndicator::kCheckmate; },
            *result);
      } else if (isCheck(position)) {
        std::visit(
            [](auto& arg) { arg.check_indicator = CheckIndicator::kCheck; },
            *result);
      }
    }

    return result;
  }

  static void undoMove(Position& position, const MoveRecord& move) {
    std::visit([&](const auto& arg) { undoMove(position, arg); }, move);
  }

 private:
  static auto executeMove(Position& position, const SanMove& move)
      -> std::expected<MoveRecord, MoveError> {
    return std::visit(
        [&](const auto& arg) { return executeMove(position, arg); }, move);
  }

  static auto executeMove(Position& position, const SanNormalMove& move)
      -> std::expected<MoveRecord, MoveError> {
    auto uci = uciFromSan(position, move, position.activeColor());

    if (!uci) return std::unexpected(uci.error());

    return executeNormalMove(position, *uci);
  }

  static auto executeMove(Position& position, const SanCastlingMove& move)
      -> std::expected<MoveRecord, MoveError> {
    return executeCastling(position, move.side);
  }

  static auto executeMove(Position& position, const UciMove& move)
      -> std::expected<MoveRecord, MoveError> {
    auto castling_side = castlingSideFromUci(position.piecePlacement(), move,
                                             position.activeColor());

    if (castling_side) return executeCastling(position, *castling_side);

    if (auto err = uciMoveError(position, rawMoveFromUci(move))) {
      return std::unexpected(*err);
    }

    return executeNormalMove(position, move);
  }

  static auto executeNormalMove(Position& position, const UciMove& uci)
      -> std::expected<MoveRecord, MoveError> {
    const auto& castling_rights = position.castlingRights();
    const auto& active_color = position.activeColor();
    const auto& en_passant_file = position.en_passant_file_;
    const auto& halfmove_clock = position.halfmoveClock();

    if (auto error = overflowError(position, rawMoveFromUci(uci))) {
      return std::unexpected(error.value());
    }

    auto destination_piece =
        pieceAt(position.piece_placement_, uci.destination);

    auto origin_piece = pieceAt(position.piece_placement_, uci.origin);
    if (!origin_piece) return std::unexpected(MoveError::kNoPieceAtOrigin);
    if (origin_piece->color != active_color) {
      return std::unexpected(MoveError::kWrongPieceColorAtOrigin);
    }

    bool const is_pawn_move = origin_piece->type == PieceType::kPawn;
    bool const is_normal_capture = destination_piece.has_value();
    auto partial_origin = partialOriginFromMove(position, rawMoveFromUci(uci));

    auto result =
        PiecePlacementModifier::doNormalMove(position.piece_placement_, uci);

    if (result) {
      auto previous_castling_rights = castling_rights;

      NormalMoveRecord move_record = {
          .piece_type = origin_piece->type,
          .partial_origin = *partial_origin,
          .uci_move = uci,
          .previous_en_passant_file = en_passant_file,
          .previous_halfmove_clock = halfmove_clock,
      };

      if (is_normal_capture) {
        move_record.captured_piece_type = destination_piece->type;
      }

      // Handle en passant capture
      bool const is_en_passant_capture =
          is_pawn_move && position.enPassantTargetSquare() == uci.destination;
      if (is_en_passant_capture) {
        if (auto captured_pawn_square =
                enPassantCapturedPawnSquare(uci.destination, active_color)) {
          move_record.is_en_passant_capture = true;
          PiecePlacementModifier::setPieceAt(
              position.piece_placement_, *captured_pawn_square, std::nullopt);
        }
      }

      // Update castling rights
      updateCastlingRights(position, rawMoveFromUci(uci));

      // Update en passant
      if (is_pawn_move) {
        updateEnPassantTargetForPawnMove(position, rawMoveFromUci(uci));
      } else {
        position.resetEnPassantFile();
      }

      // Update move counters
      position.incrementFullmoveNumber();
      if (is_pawn_move || is_normal_capture) {
        position.resetHalfmoveClock();
      } else {
        position.incrementHalfmoveClock();
      }

      if (previous_castling_rights != castling_rights) {
        move_record.previous_castling_rights = previous_castling_rights;
      }

      return move_record;
    }

    return std::unexpected(result.error());
  }

  static void updateCastlingRights(Position& position, const RawMove& move) {
    for (auto color : {Color::kBlack, Color::kWhite}) {
      if (affectsKingsideCastling(move, color)) {
        position.castling_rights_.disable(CastlingSide::kKingside, color);
      }
      if (affectsQueensideCastling(move, color)) {
        position.castling_rights_.disable(CastlingSide::kQueenside, color);
      }
    }
  }

  static void updateEnPassantTargetForPawnMove(Position& position,
                                               const RawMove& move) {
    position.resetEnPassantFile();

    const auto& active_color = position.activeColor();
    const auto& origin = move.origin;
    const auto& dest = move.destination;

    bool const is_double_push =
        isPawnStartingRank(origin.rank, active_color) &&
        isDoublePawnPushTargetRank(dest.rank, active_color) &&
        origin.file == dest.file;

    if (!is_double_push) return;

    position.en_passant_file_ = dest.file;
  }

  static auto executeCastling(Position& position, const CastlingSide& side)
      -> std::expected<MoveRecord, MoveError> {
    const auto& castling_rights = position.castlingRights();
    const auto& active_color = position.activeColor();
    const auto& en_passant_file = position.en_passant_file_;

    if (auto error = overflowError(position)) {
      return std::unexpected(error.value());
    }

    if (!castling_rights.canCastle(side, active_color)) {
      return std::unexpected(MoveError::kKingOrRookMoved);
    }

    auto result = PiecePlacementModifier::doCastling(position.piece_placement_,
                                                     side, active_color);

    if (result) {
      CastlingMoveRecord move = {
          .side = side,
          .color = active_color,
          .check_indicator = std::nullopt,
          .previous_castling_rights = castling_rights,
          .previous_en_passant_file = en_passant_file,
      };

      position.castling_rights_.disable(active_color);
      position.resetEnPassantFile();
      position.incrementMoveCounters();

      return move;
    }

    return std::unexpected(result.error());
  }

  template <typename T>
  static void undoCommonMoveEffects(Position& position, const T& move) {
    position.en_passant_file_ = move.previous_en_passant_file;
    if (position.activeColor() == Color::kWhite) position.fullmove_number_ -= 1;

    position.toggleActiveColor();
  }

  static void undoMove(Position& position, const NormalMoveRecord& move) {
    undoNormalMove(position, move);
    position.halfmove_clock_ = move.previous_halfmove_clock;
    if (move.previous_castling_rights) {
      position.castling_rights_ = *(move.previous_castling_rights);
    }

    undoCommonMoveEffects(position, move);
  }

  static void undoMove(Position& position, const CastlingMoveRecord& move) {
    undoCastling(position, move.side);
    position.halfmove_clock_ -= 1;
    position.castling_rights_ = move.previous_castling_rights;

    undoCommonMoveEffects(position, move);
  }

  static void undoNormalMove(Position& position, const NormalMoveRecord& move) {
    const auto& destination = move.uci_move.destination;
    const auto& captured_piece_type = move.captured_piece_type;
    const auto mover_color = !position.activeColor();
    const auto opponent_color = !mover_color;
    auto& piece_placement = position.piece_placement_;

    bool const was_promotion = move.piece_type == PieceType::kPawn &&
                               destination.rank == promotionRank(mover_color);

    if (was_promotion) {
      auto piece = Piece(PieceType::kPawn, mover_color);
      PiecePlacementModifier::setPieceAt(piece_placement, destination, piece);
    }

    PiecePlacementModifier::relocatePiece(
        piece_placement, reverse(rawMoveFromUci(move.uci_move)));

    if (captured_piece_type) {
      auto captured_piece = Piece(*captured_piece_type, opponent_color);
      PiecePlacementModifier::setPieceAt(piece_placement, destination,
                                         captured_piece);
    }

    if (move.is_en_passant_capture) {
      if (auto captured_pawn_square =
              enPassantCapturedPawnSquare(destination, mover_color)) {
        auto captured_piece = Piece(PieceType::kPawn, opponent_color);
        PiecePlacementModifier::setPieceAt(
            piece_placement, *captured_pawn_square, captured_piece);
      }
    }
  }

  static void undoCastling(Position& position, const CastlingSide& side) {
    const auto previous_color = !position.activeColor();
    auto& piece_placement = position.piece_placement_;

    auto moves = castlingMoves(side, previous_color);

    PiecePlacementModifier::relocatePiece(piece_placement,
                                          reverse(moves.king_move));
    PiecePlacementModifier::relocatePiece(piece_placement,
                                          reverse(moves.rook_move));
  }
};

}  // namespace chesscxx::internal

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_POSITION_MODIFIER_H_
