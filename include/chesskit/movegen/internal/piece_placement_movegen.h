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

// Range Adaptors
template <typename T>
inline auto squaresWith(PiecePlacement pp, const T& condition) {
  return std::views::filter([pp, condition](const auto& square) {
    return hasPieceAt(pp, square, condition);
  });
}

template <typename T>
inline auto squaresWithout(PiecePlacement pp, const T& condition) {
  return std::views::filter([pp, condition](const auto& square) {
    return !hasPieceAt(pp, square, condition);
  });
}

inline bool canSlideInDirection(const PieceType& type,
                                const SlidingDirection& direction) {
  bool isDiagonal = (direction == SlidingDirection::kDiagonal);
  return type == PieceType::kQueen ||
         type == (isDiagonal ? PieceType::kBishop : PieceType::kRook);
}
inline bool matches(const Piece& piece,
                    const PieceSpecification<PieceType>& pieceSpec) {
  return piece.type == pieceSpec.spec && piece.color == pieceSpec.color;
}
inline bool matches(const Piece& piece,
                    const PieceSpecification<SlidingDirection>& pieceSpec) {
  return canSlideInDirection(piece.type, pieceSpec.spec) &&
         piece.color == pieceSpec.color;
}
template <typename SearchType>
inline auto firstMatchingPiece(
    PiecePlacement pp, const PieceSpecification<SearchType>& pieceSpec) {
  return std::views::drop_while(
             [pp](const Square& sq) { return !hasPieceAt(pp, sq); }) |
         std::views::take(1) |
         std::views::filter([pp, pieceSpec](const Square& sq) {
           return matches(*pieceAt(pp, sq), pieceSpec);
         });
}
template <typename SearchType>
inline auto firstMatchingPieceInEachRange(
    PiecePlacement pp, const PieceSpecification<SearchType>& pieceSpec) {
  return std::views::transform([pp, pieceSpec](auto&& range) {
    return std::forward<decltype(range)>(range) |
           firstMatchingPiece<SearchType>(pp, pieceSpec);
  });
}

/*
inline auto takeWhileEmptyOrCapture(const PiecePlacement pp, Color color) {
  return std::views::take_while([pp, color](const Square& sq) {
    static bool foundCapture = false;
    if (foundCapture) return false;
    auto piece = pieceAt(pp, sq);
    if (piece) {
      if (piece->color == color) return false;
      foundCapture = true;
    }
    return true;
  });
}
inline auto takeWhileEmptyOrCaptureInEachRange(const PiecePlacement pp,
                                               Color color) {
  return std::views::transform([pp, color](auto&& range) {
    return std::forward<decltype(range)>(range) |
           takeWhileEmptyOrCapture(pp, color);
  });
}
*/

inline std::generator<Square> pseudoLegalKnightMoves(const PiecePlacement pp,
                                                     Square square,
                                                     Color color) {
  co_yield std::ranges::elements_of(knightMoves(square) |
                                    squaresWithout(pp, color));
}

inline std::generator<Square> pseudoLegalKingMoves(const PiecePlacement pp,
                                                   Square square, Color color) {
  co_yield std::ranges::elements_of(kingMoves(square) |
                                    squaresWithout(pp, color));
}

inline std::generator<Square> pseudoLegalPawnPushs(const PiecePlacement pp,
                                                   Square square, Color color) {
  co_yield std::ranges::elements_of(
      pawnSlidingMove(square, color) |
      std::views::take_while(
          [pp](const Square& sq) { return !hasPieceAt(pp, sq); }));
}

inline std::generator<Square> takeWhileEmptyOrCapture(
    std::generator<Square>&& gen, const PiecePlacement pp, Color color) {
  for (auto square : gen) {
    auto piece = pieceAt(pp, square);
    if (piece) {
      if (piece->color != color) co_yield square;
      co_return;
    }

    co_yield square;
  }
}

inline std::generator<Square> pseudoLegalRookMoves(const PiecePlacement pp,
                                                   Square square, Color color) {
  co_yield std::ranges::elements_of(
      rookSlidingMoves(square) |
      std::views::transform(
          [pp, square, color](std::generator<Square>&& sliding_move) {
            return takeWhileEmptyOrCapture(std::move(sliding_move), pp, color);
          }) |
      std::views::join);
  /*
  co_yield std::ranges::elements_of(
      rookSlidingMoves(square) | takeWhileEmptyOrCaptureInEachRange(pp, color) |
      std::views::join);*/
}

inline std::generator<Square> pseudoLegalBishopMoves(const PiecePlacement pp,
                                                     Square square,
                                                     Color color) {
  co_yield std::ranges::elements_of(
      bishopSlidingMoves(square) |
      std::views::transform(
          [pp, square, color](std::generator<Square>&& sliding_move) {
            return takeWhileEmptyOrCapture(std::move(sliding_move), pp, color);
          }) |
      std::views::join);
  /*
  co_yield std::ranges::elements_of(
      bishopSlidingMoves(square) |
      takeWhileEmptyOrCaptureInEachRange(pp, color) | std::views::join);*/
}

inline std::generator<Square> pseudoLegalQueenMoves(const PiecePlacement pp,
                                                    Square square,
                                                    Color color) {
  co_yield std::ranges::elements_of(
      queenSlidingMoves(square) |
      std::views::transform(
          [pp, square, color](std::generator<Square>&& sliding_move) {
            return takeWhileEmptyOrCapture(std::move(sliding_move), pp, color);
          }) |
      std::views::join);
  /*
  co_yield std::ranges::elements_of(
      queenSlidingMoves(square) |
      takeWhileEmptyOrCaptureInEachRange(pp, color) | std::views::join);*/
}

inline std::generator<Square> pawnsAttacking(PiecePlacement pp, Square square,
                                             Color color) {
  co_yield std::ranges::elements_of(
      pawnCaptures(square, !color) |
      squaresWith(pp, Piece{PieceType::kPawn, color}));
}

inline std::generator<Square> pawnMovingTo(PiecePlacement pp, Square square,
                                           Color color) {
  if (hasPieceAt(pp, square)) co_return;

  co_yield std::ranges::elements_of(
      pawnReverseSlidingMove(square, color) |
      firstMatchingPiece<PieceType>(
          pp, PieceSpecification<PieceType>{PieceType::kPawn, color}));
}

inline std::generator<Square> knightsReaching(PiecePlacement pp, Square square,
                                              Color color) {
  co_yield std::ranges::elements_of(
      knightMoves(square) | squaresWith(pp, Piece{PieceType::kKnight, color}));
}

inline std::generator<Square> kingsReaching(PiecePlacement pp, Square square,
                                            Color color) {
  co_yield std::ranges::elements_of(
      kingMoves(square) | squaresWith(pp, Piece{PieceType::kKing, color}));
}

inline std::generator<Square> orthogonalSlidersReaching(PiecePlacement pp,
                                                        Square square,
                                                        Color color) {
  co_yield std::ranges::elements_of(
      rookSlidingMoves(square) |
      firstMatchingPieceInEachRange<SlidingDirection>(
          pp, {SlidingDirection::kOrthogonal, color}) |
      std::views::join);
}

inline std::generator<Square> rooksReaching(PiecePlacement pp, Square square,
                                            Color color) {
  co_yield std::ranges::elements_of(
      rookSlidingMoves(square) |
      firstMatchingPieceInEachRange<PieceType>(pp, {PieceType::kRook, color}) |
      std::views::join);
}

inline std::generator<Square> diagonalSlidersReaching(PiecePlacement pp,
                                                      Square square,
                                                      Color color) {
  co_yield std::ranges::elements_of(
      bishopSlidingMoves(square) |
      firstMatchingPieceInEachRange<SlidingDirection>(
          pp, {SlidingDirection::kDiagonal, color}) |
      std::views::join);
}

inline std::generator<Square> bishopsReaching(PiecePlacement pp, Square square,
                                              Color color) {
  co_yield std::ranges::elements_of(bishopSlidingMoves(square) |
                                    firstMatchingPieceInEachRange<PieceType>(
                                        pp, {PieceType::kBishop, color}) |
                                    std::views::join);
}

inline std::generator<Square> queensReaching(PiecePlacement pp, Square square,
                                             Color color) {
  co_yield std::ranges::elements_of(
      queenSlidingMoves(square) |
      firstMatchingPieceInEachRange<PieceType>(pp, {PieceType::kQueen, color}) |
      std::views::join);
}

inline std::generator<Square> piecesAttacking(PiecePlacement pp, Square square,
                                              Color color) {
  using std::ranges::elements_of;

  co_yield elements_of(pawnsAttacking(pp, square, color));
  co_yield elements_of(knightsReaching(pp, square, color));
  co_yield elements_of(kingsReaching(pp, square, color));
  co_yield elements_of(orthogonalSlidersReaching(pp, square, color));
  co_yield elements_of(diagonalSlidersReaching(pp, square, color));
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_INTERNAL_PIECE_PLACEMENT_MOVEGEN_H_
