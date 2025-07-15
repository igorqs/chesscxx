#ifndef CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_INTERNAL_SQUARE_MOVEGEN_H_
#define CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_INTERNAL_SQUARE_MOVEGEN_H_

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <generator>
#include <optional>
#include <ranges>
#include <utility>

#include "../../color.h"
#include "../../core/internal/square.h"
#include "../../file.h"
#include "../../rank.h"
#include "../../square.h"

namespace chesskit::internal {

enum class Origin {
  kKeep,
  kSkip,
};

inline auto validMoves(Square sq) {
  return std::views::transform(
             [sq](auto offset) { return shiftSquare(sq, offset); }) |
         std::views::filter(
             [](auto optional) { return optional.has_value(); }) |
         std::views::transform([](auto optional) { return *optional; });
}

inline auto diagonalProjection(const Square& sq, const Square& corner)
    -> Square {
  SquareOffset offset = calculateOffset(corner, sq);
  bool const sameSign =
      std::signbit(offset.fileOffset) == std::signbit(offset.rankOffset);
  int8_t const multiplier = sameSign ? 1 : -1;

  if (std::abs(offset.fileOffset) < std::abs(offset.rankOffset)) {
    offset.rankOffset = offset.fileOffset * multiplier;
  } else {
    offset.fileOffset = offset.rankOffset * multiplier;
  }

  auto projection = shiftSquare(sq, offset);
  if (projection) return *projection;

  std::unreachable();
}

inline auto isOrthogonal(const SquareOffset& offset) -> bool {
  return offset.fileOffset == 0 || offset.rankOffset == 0;
}

inline auto isDiagonal(const SquareOffset& offset) -> bool {
  return std::abs(offset.fileOffset) == std::abs(offset.rankOffset);
}

inline auto isStraightLine(const SquareOffset& offset) -> bool {
  return isOrthogonal(offset) || isDiagonal(offset);
}

inline auto slidingIncrement(const SquareOffset& offset)
    -> std::optional<SquareOffset> {
  if (!isStraightLine(offset)) return std::nullopt;

  static constexpr int lo = -1;
  static constexpr int hi = 1;

  return SquareOffset{.fileOffset = std::clamp(offset.fileOffset, lo, hi),
                      .rankOffset = std::clamp(offset.rankOffset, lo, hi)};
}

inline auto traversedSquares(Square origin, Square destination,
                             Origin originPolicy = Origin::kKeep)
    -> std::generator<Square> {
  if (originPolicy == Origin::kSkip && origin == destination) co_return;

  auto increment = calculateOffset(destination, origin);

  if (const auto& slidingInc = slidingIncrement(increment)) {
    increment = *slidingInc;
  }

  std::optional<Square> square = origin;
  if (originPolicy == Origin::kSkip) square = shiftSquare(*square, increment);

  for (; square && *square != destination;
       square = shiftSquare(*square, increment)) {
    co_yield *square;
  }

  co_yield destination;
}

inline auto fileNeighbors(Square sq) -> std::generator<Square> {
  static constexpr std::array<SquareOffset, 2> kOffsets = {
      {{.fileOffset = 1, .rankOffset = 0},
       {.fileOffset = -1, .rankOffset = 0}}};

  co_yield std::ranges::elements_of(kOffsets | validMoves(sq));
}

inline auto knightMoves(Square sq) -> std::generator<Square> {
  static constexpr std::array<SquareOffset, 8> kOffsets = {
      {{.fileOffset = 2, .rankOffset = 1},
       {.fileOffset = 2, .rankOffset = -1},
       {.fileOffset = 1, .rankOffset = 2},
       {.fileOffset = 1, .rankOffset = -2},
       {.fileOffset = -1, .rankOffset = 2},
       {.fileOffset = -1, .rankOffset = -2},
       {.fileOffset = -2, .rankOffset = 1},
       {.fileOffset = -2, .rankOffset = -1}}};

  co_yield std::ranges::elements_of(kOffsets | validMoves(sq));
}

inline auto pawnCaptures(Square sq, Color color) -> std::generator<Square> {
  static constexpr std::array<SquareOffset, 2> kWhiteOffsets = {
      {{.fileOffset = 1, .rankOffset = -1},
       {.fileOffset = -1, .rankOffset = -1}}};
  static constexpr std::array<SquareOffset, 2> kBlackOffsets = {
      {{.fileOffset = 1, .rankOffset = 1},
       {.fileOffset = -1, .rankOffset = 1}}};

  co_yield std::ranges::elements_of(
      ((color == Color::kWhite) ? kWhiteOffsets : kBlackOffsets) |
      validMoves(sq));
}

inline auto pawnReverseSlidingMove(Square sq, Color color)
    -> std::generator<Square> {
  if (auto source = farthestPawnPushSource(sq, color)) {
    co_yield std::ranges::elements_of(
        traversedSquares(sq, *source, Origin::kSkip));
  }
}

inline auto pawnSlidingMove(Square sq, Color color) -> std::generator<Square> {
  if (auto destination = farthestPawnPush(sq, color)) {
    co_yield std::ranges::elements_of(
        traversedSquares(sq, *destination, Origin::kSkip));
  }
}

inline auto kingMoves(Square sq) -> std::generator<Square> {
  static constexpr std::array<SquareOffset, 8> kOffsets = {
      {{.fileOffset = 1, .rankOffset = 1},
       {.fileOffset = 1, .rankOffset = 0},
       {.fileOffset = 1, .rankOffset = -1},
       {.fileOffset = 0, .rankOffset = 1},
       {.fileOffset = 0, .rankOffset = -1},
       {.fileOffset = -1, .rankOffset = 1},
       {.fileOffset = -1, .rankOffset = 0},
       {.fileOffset = -1, .rankOffset = -1}}};

  co_yield std::ranges::elements_of(kOffsets | validMoves(sq));
}

inline auto rookSlidingMoves(Square sq)
    -> std::generator<std::generator<Square>> {
  co_yield (
      traversedSquares(sq, {.file = sq.file, .rank = Rank::k1}, Origin::kSkip));
  co_yield (
      traversedSquares(sq, {.file = sq.file, .rank = Rank::k8}, Origin::kSkip));
  co_yield (
      traversedSquares(sq, {.file = File::kA, .rank = sq.rank}, Origin::kSkip));
  co_yield (
      traversedSquares(sq, {.file = File::kH, .rank = sq.rank}, Origin::kSkip));
}

inline auto bishopSlidingMoves(Square sq)
    -> std::generator<std::generator<Square>> {
  constexpr static std::array<Square, 4> corners = {
      {{.file = File::kA, .rank = Rank::k8},
       {.file = File::kH, .rank = Rank::k8},
       {.file = File::kA, .rank = Rank::k1},
       {.file = File::kH, .rank = Rank::k1}}};

  for (const auto& corner : corners) {
    auto projection = diagonalProjection(sq, corner);
    co_yield (traversedSquares(sq, projection, Origin::kSkip));
  }
}

inline auto queenSlidingMoves(Square sq)
    -> std::generator<std::generator<Square>> {
  co_yield std::ranges::elements_of(rookSlidingMoves(sq));
  co_yield std::ranges::elements_of(bishopSlidingMoves(sq));
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_INTERNAL_SQUARE_MOVEGEN_H_
