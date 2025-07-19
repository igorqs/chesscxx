#ifndef CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_INTERNAL_POSITION_MOVEGEN_H_
#define CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_INTERNAL_POSITION_MOVEGEN_H_

#include <array>
#include <generator>
#include <optional>
#include <ranges>
#include <utility>

#include "../../castling_side.h"
#include "../../color.h"
#include "../../core/internal/piece_placement.h"
#include "../../core/internal/piece_placement_piece_at.h"
#include "../../core/internal/rank.h"
#include "../../core/internal/raw_move.h"
#include "../../piece.h"
#include "../../piece_type.h"
#include "../../position.h"
#include "../../square.h"
#include "../../uci_move.h"
#include "piece_placement_movegen.h"
#include "square_movegen.h"

namespace chesskit::internal {

inline auto pseudoLegalPawnCaptures(const Position& position, Square square,
                                    Color color) -> std::generator<Square> {
  co_yield std::ranges::elements_of(
      pawnCaptures(square, color) |
      std::views::filter([position, color](const Square& square) {
        return hasPieceAt(position.piecePlacement(), square, !color) ||
               square == position.enPassantTargetSquare();
      }));
}

inline auto pseudoLegalPawnMoves(const Position& position, Square square,
                                 Color color) -> std::generator<Square> {
  co_yield std::ranges::elements_of(
      pseudoLegalPawnPushs(position.piecePlacement(), square, color));
  co_yield std::ranges::elements_of(
      pseudoLegalPawnCaptures(position, square, color));
}

inline auto pseudoLegalMoves(const Position& position, Square square)
    -> std::generator<Square> {
  const auto& piece_placement = position.piecePlacement();

  auto piece = pieceAt(piece_placement, square);
  if (!piece) co_return;

  using std::ranges::elements_of;

  switch (piece->type) {
    case PieceType::kPawn:
      co_yield elements_of(
          pseudoLegalPawnMoves(position, square, piece->color));
      co_return;
    case PieceType::kKnight:
      co_yield elements_of(
          pseudoLegalKnightMoves(piece_placement, square, piece->color));
      co_return;
    case PieceType::kKing:
      co_yield elements_of(
          pseudoLegalKingMoves(piece_placement, square, piece->color));
      co_return;
    case PieceType::kRook:
      co_yield elements_of(
          pseudoLegalRookMoves(piece_placement, square, piece->color));
      co_return;
    case PieceType::kBishop:
      co_yield elements_of(
          pseudoLegalBishopMoves(piece_placement, square, piece->color));
      co_return;
    case PieceType::kQueen:
      co_yield elements_of(
          pseudoLegalQueenMoves(piece_placement, square, piece->color));
      co_return;
    default:
      std::unreachable();
  }
}

inline auto pseudoLegalMoves(const Position& position)
    -> std::generator<RawMove> {
  using std::ranges::elements_of;

  const auto& piece_placement = position.piecePlacement();
  const auto& color = position.activeColor();

  auto location_to_pseudo_legal_moves =
      [&](Square piece_location) -> std::generator<RawMove> {
    co_yield elements_of(pseudoLegalMoves(position, piece_location) |
                         std::views::transform([&](const Square& destination) {
                           return RawMove(piece_location, destination);
                         }));
  };

  co_yield elements_of(piece_placement.pieceLocations().at(color) |
                       std::views::values | std::views::join |
                       std::views::transform(location_to_pseudo_legal_moves) |
                       std::views::join);
}

inline auto legalRawMoves(const Position& position) -> std::generator<RawMove> {
  using std::ranges::elements_of;

  co_yield elements_of(pseudoLegalMoves(position) |
                       std::views::filter([position](const RawMove& move) {
                         return !moveResultsInSelfCheck(
                             position.piecePlacement(), move,
                             position.activeColor());
                       }));
}

inline auto legalCastlings(const Position& position)
    -> std::generator<CastlingSide> {
  constexpr static std::array<CastlingSide, 2> kSides = {
      CastlingSide::kKingside, CastlingSide::kQueenside};

  const auto& color = position.activeColor();

  for (const auto& side : kSides) {
    bool const can_castle = position.castlingRights().canCastle(side, color);
    if (!can_castle) continue;

    if (!castlingError(position.piecePlacement(), side, color)) co_yield side;
  }
}

inline auto uciPromotions(const RawMove& raw_move) -> std::generator<UciMove> {
  constexpr static std::array<PromotablePieceType, 4> kPromotions = {
      PromotablePieceType::kKnight, PromotablePieceType::kBishop,
      PromotablePieceType::kRook, PromotablePieceType::kQueen};

  for (const auto& promotion : kPromotions) {
    co_yield UciMove(raw_move.origin, raw_move.destination, promotion);
  }
}

inline auto legalMoves(Position position) -> std::generator<UciMove> {
  using std::ranges::elements_of;

  co_yield elements_of(
      legalCastlings(position) |
      std::views::transform([&position](const auto& side) {
        auto raw_move = castlingMoves(side, position.activeColor()).king_move;
        return UciMove(raw_move.origin, raw_move.destination, std::nullopt);
      }));

  co_yield elements_of(
      legalRawMoves(position) |
      std::views::transform([&position](const auto& raw_move)
                                -> std::generator<UciMove> {
        auto piece = pieceAt(position.piecePlacement(), raw_move.origin);
        bool const is_pawn = piece && piece->type == PieceType::kPawn;
        bool const is_promotion_rank =
            raw_move.destination.rank == promotionRank(position.activeColor());
        bool const is_promotion = is_pawn && is_promotion_rank;

        if (is_promotion) {
          co_yield elements_of(uciPromotions(raw_move));
          co_return;
        }

        co_yield UciMove(raw_move.origin, raw_move.destination, std::nullopt);
      }) |
      std::views::join);

  co_return;
}

inline auto hasLegalMove(const Position& position) -> bool {
  auto moves = legalMoves(position);
  return moves.begin() != moves.end();
}

inline auto pawnsCapturing(const Position& position, Square square, Color color)
    -> std::generator<Square> {
  bool const has_opponent =
      hasPieceAt(position.piecePlacement(), square, !color);
  bool const is_en_passant_target =
      (square == position.enPassantTargetSquare());

  if (!has_opponent && !is_en_passant_target) co_return;

  co_yield std::ranges::elements_of(
      pawnsAttacking(position.piecePlacement(), square, color));
}

inline auto pawnsReaching(const Position& position, Square square, Color color)
    -> std::generator<Square> {
  co_yield std::ranges::elements_of(pawnsCapturing(position, square, color));
  co_yield std::ranges::elements_of(
      pawnMovingTo(position.piecePlacement(), square, color));
}

inline auto piecesReaching(const Position& position, Square square, Piece piece)
    -> std::generator<Square> {
  const auto& piece_placement = position.piecePlacement();

  auto piece_at_target_square = pieceAt(piece_placement, square);
  if (piece_at_target_square && piece_at_target_square->color == piece.color) {
    co_return;
  }

  using std::ranges::elements_of;

  switch (piece.type) {
    case PieceType::kPawn:
      co_yield elements_of(pawnsReaching(position, square, piece.color));
      co_return;
    case PieceType::kKnight:
      co_yield elements_of(
          knightsReaching(piece_placement, square, piece.color));
      co_return;
    case PieceType::kKing:
      co_yield elements_of(kingsReaching(piece_placement, square, piece.color));
      co_return;
    case PieceType::kRook:
      co_yield elements_of(rooksReaching(piece_placement, square, piece.color));
      co_return;
    case PieceType::kBishop:
      co_yield elements_of(
          bishopsReaching(piece_placement, square, piece.color));
      co_return;
    case PieceType::kQueen:
      co_yield elements_of(
          queensReaching(piece_placement, square, piece.color));
      co_return;
    default:
      std::unreachable();
  }
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_INTERNAL_POSITION_MOVEGEN_H_
