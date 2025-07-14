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

inline std::generator<Square> pseudoLegalPawnCaptures(const Position position,
                                                      Square square,
                                                      Color color) {
  co_yield std::ranges::elements_of(
      pawnCaptures(square, color) |
      std::views::filter([position, color](const Square& sq) {
        return hasPieceAt(position.piecePlacement(), sq, !color) ||
               sq == position.enPassantTargetSquare();
      }));
}

inline std::generator<Square> pseudoLegalPawnMoves(const Position position,
                                                   Square square, Color color) {
  co_yield std::ranges::elements_of(
      pseudoLegalPawnPushs(position.piecePlacement(), square, color));
  co_yield std::ranges::elements_of(
      pseudoLegalPawnCaptures(position, square, color));
}

inline std::generator<Square> pseudoLegalMoves(const Position position,
                                               Square square) {
  const auto& pp = position.piecePlacement();

  auto piece = pieceAt(pp, square);
  if (!piece) co_return;

  using std::ranges::elements_of;

  switch (piece->type) {
    case PieceType::kPawn:
      co_yield elements_of(
          pseudoLegalPawnMoves(position, square, piece->color));
      co_return;
    case PieceType::kKnight:
      co_yield elements_of(pseudoLegalKnightMoves(pp, square, piece->color));
      co_return;
    case PieceType::kKing:
      co_yield elements_of(pseudoLegalKingMoves(pp, square, piece->color));
      co_return;
    case PieceType::kRook:
      co_yield elements_of(pseudoLegalRookMoves(pp, square, piece->color));
      co_return;
    case PieceType::kBishop:
      co_yield elements_of(pseudoLegalBishopMoves(pp, square, piece->color));
      co_return;
    case PieceType::kQueen:
      co_yield elements_of(pseudoLegalQueenMoves(pp, square, piece->color));
      co_return;
    default:
      std::unreachable();
  }
}

inline std::generator<RawMove> pseudoLegalMoves(const Position position) {
  using std::ranges::elements_of;

  const auto& pp = position.piecePlacement();
  const auto& color = position.activeColor();

  auto locationToPseudoLegalMoves =
      [&](Square pieceLocation) -> std::generator<RawMove> {
    co_yield elements_of(pseudoLegalMoves(position, pieceLocation) |
                         std::views::transform([&](const Square& destination) {
                           return RawMove(pieceLocation, destination);
                         }));
  };

  co_yield elements_of(
      pp.pieceLocations().at(color) | std::views::values | std::views::join |
      std::views::transform(locationToPseudoLegalMoves) | std::views::join);
}

inline std::generator<RawMove> legalRawMoves(const Position position) {
  using std::ranges::elements_of;

  co_yield elements_of(pseudoLegalMoves(position) |
                       std::views::filter([position](const RawMove& move) {
                         return !moveResultsInSelfCheck(
                             position.piecePlacement(), move,
                             position.activeColor());
                       }));
}

inline std::generator<CastlingSide> legalCastlings(const Position position) {
  constexpr static std::array<CastlingSide, 2> sides = {
      CastlingSide::kKingside, CastlingSide::kQueenside};

  const auto& color = position.activeColor();

  for (const auto& side : sides) {
    bool can_castle = position.castlingRights().canCastle(side, color);
    if (!can_castle) continue;

    if (!castlingError(position.piecePlacement(), side, color)) co_yield side;
  }
}

inline std::generator<UciMove> uciPromotions(const RawMove& raw_move) {
  constexpr static std::array<PromotablePieceType, 4> promotions = {
      PromotablePieceType::kKnight, PromotablePieceType::kBishop,
      PromotablePieceType::kRook, PromotablePieceType::kQueen};

  for (const auto& promotion : promotions) {
    co_yield UciMove(raw_move.origin, raw_move.destination, promotion);
  }
}

template <typename T = UciMove>
inline std::generator<T> legalMoves(const Position);

template <>
inline std::generator<UciMove> legalMoves(const Position position) {
  using std::ranges::elements_of;

  co_yield elements_of(
      legalCastlings(position) |
      std::views::transform([&position](const auto& side) {
        auto raw_move = castlingMoves(side, position.activeColor()).kingMove;
        return UciMove(raw_move.origin, raw_move.destination, std::nullopt);
      }));

  co_yield elements_of(
      legalRawMoves(position) |
      std::views::transform(
          [&position](const auto& raw_move) -> std::generator<UciMove> {
            auto piece = pieceAt(position.piecePlacement(), raw_move.origin);
            bool isPawn = piece && piece->type == PieceType::kPawn;
            bool isPromotionRank = raw_move.destination.rank ==
                                   promotionRank(position.activeColor());
            bool isPromotion = isPawn && isPromotionRank;

            if (isPromotion) {
              co_yield elements_of(uciPromotions(raw_move));
              co_return;
            }

            co_yield UciMove(raw_move.origin, raw_move.destination,
                             std::nullopt);
          }) |
      std::views::join);

  co_return;
}

inline bool hasLegalMove(const Position position) {
  auto moves = legalMoves(position);
  return moves.begin() != moves.end();
}

inline std::generator<Square> pawnsCapturing(Position position, Square square,
                                             Color color) {
  bool hasOpponent = hasPieceAt(position.piecePlacement(), square, !color);
  bool isEnPassantTarget = (square == position.enPassantTargetSquare());

  if (!hasOpponent && !isEnPassantTarget) co_return;

  co_yield std::ranges::elements_of(
      pawnsAttacking(position.piecePlacement(), square, color));
}

inline std::generator<Square> pawnsReaching(Position position, Square square,
                                            Color color) {
  co_yield std::ranges::elements_of(pawnsCapturing(position, square, color));
  co_yield std::ranges::elements_of(
      pawnMovingTo(position.piecePlacement(), square, color));
}

inline std::generator<Square> piecesReaching(Position position, Square square,
                                             Piece piece) {
  const auto& pp = position.piecePlacement();

  auto pieceAtTargetSquare = pieceAt(pp, square);
  if (pieceAtTargetSquare && pieceAtTargetSquare->color == piece.color) {
    co_return;
  }

  using std::ranges::elements_of;

  switch (piece.type) {
    case PieceType::kPawn:
      co_yield elements_of(pawnsReaching(position, square, piece.color));
      co_return;
    case PieceType::kKnight:
      co_yield elements_of(knightsReaching(pp, square, piece.color));
      co_return;
    case PieceType::kKing:
      co_yield elements_of(kingsReaching(pp, square, piece.color));
      co_return;
    case PieceType::kRook:
      co_yield elements_of(rooksReaching(pp, square, piece.color));
      co_return;
    case PieceType::kBishop:
      co_yield elements_of(bishopsReaching(pp, square, piece.color));
      co_return;
    case PieceType::kQueen:
      co_yield elements_of(queensReaching(pp, square, piece.color));
      co_return;
    default:
      std::unreachable();
  }
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_INTERNAL_POSITION_MOVEGEN_H_
