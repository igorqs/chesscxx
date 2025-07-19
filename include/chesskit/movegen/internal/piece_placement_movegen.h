#ifndef CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_INTERNAL_PIECE_PLACEMENT_MOVEGEN_H_
#define CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_INTERNAL_PIECE_PLACEMENT_MOVEGEN_H_

#include <cstdint>
#include <generator>
#include <optional>
#include <ranges>

#include "../../color.h"
#include "../../core/internal/piece_placement_piece_at.h"
#include "../../piece.h"
#include "../../piece_placement.h"
#include "../../piece_type.h"
#include "../../square.h"
#include "square_movegen.h"

namespace chesskit::internal {

template <typename SearchType>
struct PieceSpecification {
  SearchType spec;
  Color color;
};

enum class SlidingDirection : uint8_t {
  kOrthogonal,
  kDiagonal,
};

template <typename T>
inline auto squaresWith(PiecePlacement piece_placement, const T& condition) {
  return std::views::filter([piece_placement, condition](const auto& square) {
    return hasPieceAt(piece_placement, square, condition);
  });
}

template <typename T>
inline auto squaresWithout(PiecePlacement piece_placement, const T& condition) {
  return std::views::filter([piece_placement, condition](const auto& square) {
    return !hasPieceAt(piece_placement, square, condition);
  });
}

inline auto canSlideInDirection(const PieceType& type,
                                const SlidingDirection& direction) -> bool {
  bool const is_diagonal = (direction == SlidingDirection::kDiagonal);
  return type == PieceType::kQueen ||
         type == (is_diagonal ? PieceType::kBishop : PieceType::kRook);
}
inline auto matches(const Piece& piece,
                    const PieceSpecification<PieceType>& piece_spec) -> bool {
  return piece.type == piece_spec.spec && piece.color == piece_spec.color;
}
inline auto matches(const Piece& piece,
                    const PieceSpecification<SlidingDirection>& piece_spec)
    -> bool {
  return canSlideInDirection(piece.type, piece_spec.spec) &&
         piece.color == piece_spec.color;
}
template <typename SearchType>
inline auto firstMatchingPiece(
    const PiecePlacement& piece_placement,
    const PieceSpecification<SearchType>& piece_spec) {
  return std::views::drop_while([piece_placement](const Square& square) {
           return !hasPieceAt(piece_placement, square);
         }) |
         std::views::take(1) |
         std::views::filter(
             [piece_placement, piece_spec](const Square& square) {
               return matches(*pieceAt(piece_placement, square), piece_spec);
             });
}
template <typename SearchType>
inline auto firstMatchingPieceInEachRange(
    const PiecePlacement& piece_placement,
    const PieceSpecification<SearchType>& piece_spec) {
  return std::views::transform([piece_placement, piece_spec](auto&& range) {
    return std::forward<decltype(range)>(range) |
           firstMatchingPiece<SearchType>(piece_placement, piece_spec);
  });
}

inline auto pseudoLegalKnightMoves(const PiecePlacement& piece_placement,
                                   Square square, Color color)
    -> std::generator<Square> {
  co_yield std::ranges::elements_of(knightMoves(square) |
                                    squaresWithout(piece_placement, color));
}

inline auto pseudoLegalKingMoves(const PiecePlacement& piece_placement,
                                 Square square, Color color)
    -> std::generator<Square> {
  co_yield std::ranges::elements_of(kingMoves(square) |
                                    squaresWithout(piece_placement, color));
}

inline auto pseudoLegalPawnPushs(const PiecePlacement& piece_placement,
                                 Square square, Color color)
    -> std::generator<Square> {
  co_yield std::ranges::elements_of(
      pawnSlidingMove(square, color) |
      std::views::take_while([piece_placement](const Square& square) {
        return !hasPieceAt(piece_placement, square);
      }));
}

inline auto takeWhileEmptyOrCapture(std::generator<Square>&& gen,
                                    const PiecePlacement& piece_placement,
                                    Color color) -> std::generator<Square> {
  for (auto square : gen) {
    auto piece = pieceAt(piece_placement, square);
    if (piece) {
      if (piece->color != color) co_yield square;
      co_return;
    }

    co_yield square;
  }
}

inline auto pseudoLegalRookMoves(const PiecePlacement& piece_placement,
                                 Square square, Color color)
    -> std::generator<Square> {
  co_yield std::ranges::elements_of(
      rookSlidingMoves(square) |
      std::views::transform([piece_placement, square,
                             color](std::generator<Square>&& sliding_move) {
        return takeWhileEmptyOrCapture(std::move(sliding_move), piece_placement,
                                       color);
      }) |
      std::views::join);
}

inline auto pseudoLegalBishopMoves(const PiecePlacement& piece_placement,
                                   Square square, Color color)
    -> std::generator<Square> {
  co_yield std::ranges::elements_of(
      bishopSlidingMoves(square) |
      std::views::transform([piece_placement, square,
                             color](std::generator<Square>&& sliding_move) {
        return takeWhileEmptyOrCapture(std::move(sliding_move), piece_placement,
                                       color);
      }) |
      std::views::join);
}

inline auto pseudoLegalQueenMoves(const PiecePlacement& piece_placement,
                                  Square square, Color color)
    -> std::generator<Square> {
  co_yield std::ranges::elements_of(
      queenSlidingMoves(square) |
      std::views::transform([piece_placement, square,
                             color](std::generator<Square>&& sliding_move) {
        return takeWhileEmptyOrCapture(std::move(sliding_move), piece_placement,
                                       color);
      }) |
      std::views::join);
}

inline auto pawnsAttacking(PiecePlacement piece_placement, Square square,
                           Color color) -> std::generator<Square> {
  co_yield std::ranges::elements_of(
      pawnCaptures(square, !color) |
      squaresWith(piece_placement,
                  Piece{.type = PieceType::kPawn, .color = color}));
}

inline auto pawnMovingTo(PiecePlacement piece_placement, Square square,
                         Color color) -> std::generator<Square> {
  if (hasPieceAt(piece_placement, square)) co_return;

  co_yield std::ranges::elements_of(
      pawnReverseSlidingMove(square, color) |
      firstMatchingPiece<PieceType>(
          piece_placement, PieceSpecification<PieceType>{
                               .spec = PieceType::kPawn, .color = color}));
}

inline auto knightsReaching(PiecePlacement piece_placement, Square square,
                            Color color) -> std::generator<Square> {
  co_yield std::ranges::elements_of(
      knightMoves(square) |
      squaresWith(piece_placement,
                  Piece{.type = PieceType::kKnight, .color = color}));
}

inline auto kingsReaching(PiecePlacement piece_placement, Square square,
                          Color color) -> std::generator<Square> {
  co_yield std::ranges::elements_of(
      kingMoves(square) |
      squaresWith(piece_placement,
                  Piece{.type = PieceType::kKing, .color = color}));
}

inline auto orthogonalSlidersReaching(PiecePlacement piece_placement,
                                      Square square, Color color)
    -> std::generator<Square> {
  co_yield std::ranges::elements_of(
      rookSlidingMoves(square) |
      firstMatchingPieceInEachRange<SlidingDirection>(
          piece_placement,
          {.spec = SlidingDirection::kOrthogonal, .color = color}) |
      std::views::join);
}

inline auto rooksReaching(PiecePlacement piece_placement, Square square,
                          Color color) -> std::generator<Square> {
  co_yield std::ranges::elements_of(
      rookSlidingMoves(square) |
      firstMatchingPieceInEachRange<PieceType>(
          piece_placement, {.spec = PieceType::kRook, .color = color}) |
      std::views::join);
}

inline auto diagonalSlidersReaching(PiecePlacement piece_placement,
                                    Square square, Color color)
    -> std::generator<Square> {
  co_yield std::ranges::elements_of(
      bishopSlidingMoves(square) |
      firstMatchingPieceInEachRange<SlidingDirection>(
          piece_placement,
          {.spec = SlidingDirection::kDiagonal, .color = color}) |
      std::views::join);
}

inline auto bishopsReaching(PiecePlacement piece_placement, Square square,
                            Color color) -> std::generator<Square> {
  co_yield std::ranges::elements_of(
      bishopSlidingMoves(square) |
      firstMatchingPieceInEachRange<PieceType>(
          piece_placement, {.spec = PieceType::kBishop, .color = color}) |
      std::views::join);
}

inline auto queensReaching(PiecePlacement piece_placement, Square square,
                           Color color) -> std::generator<Square> {
  co_yield std::ranges::elements_of(
      queenSlidingMoves(square) |
      firstMatchingPieceInEachRange<PieceType>(
          piece_placement, {.spec = PieceType::kQueen, .color = color}) |
      std::views::join);
}

inline auto piecesAttacking(PiecePlacement piece_placement, Square square,
                            Color color) -> std::generator<Square> {
  using std::ranges::elements_of;

  co_yield elements_of(pawnsAttacking(piece_placement, square, color));
  co_yield elements_of(knightsReaching(piece_placement, square, color));
  co_yield elements_of(kingsReaching(piece_placement, square, color));
  co_yield elements_of(
      orthogonalSlidersReaching(piece_placement, square, color));
  co_yield elements_of(diagonalSlidersReaching(piece_placement, square, color));
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_INTERNAL_PIECE_PLACEMENT_MOVEGEN_H_
