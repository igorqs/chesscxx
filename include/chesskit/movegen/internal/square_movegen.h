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

inline Square diagonalProjection(const Square& sq, const Square& corner) {
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

inline bool isOrthogonal(const SquareOffset& offset) {
  return offset.fileOffset == 0 || offset.rankOffset == 0;
}

inline bool isDiagonal(const SquareOffset& offset) {
  return std::abs(offset.fileOffset) == std::abs(offset.rankOffset);
}

inline bool isStraightLine(const SquareOffset& offset) {
  return isOrthogonal(offset) || isDiagonal(offset);
}

inline std::optional<SquareOffset> slidingIncrement(
    const SquareOffset& offset) {
  if (!isStraightLine(offset)) return std::nullopt;

  static constexpr int lo = -1;
  static constexpr int hi = 1;

  return SquareOffset{std::clamp(offset.fileOffset, lo, hi),
                      std::clamp(offset.rankOffset, lo, hi)};
}

inline std::generator<Square> traversedSquares(
    Square origin, Square destination, Origin originPolicy = Origin::kKeep) {
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

inline std::generator<Square> fileNeighbors(Square sq) {
  static constexpr std::array<SquareOffset, 2> kOffsets = {{{1, 0}, {-1, 0}}};

  co_yield std::ranges::elements_of(kOffsets | validMoves(sq));
}

inline std::generator<Square> knightMoves(Square sq) {
  static constexpr std::array<SquareOffset, 8> kOffsets = {
      {{2, 1}, {2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {-2, 1}, {-2, -1}}};

  co_yield std::ranges::elements_of(kOffsets | validMoves(sq));
}

inline std::generator<Square> pawnCaptures(Square sq, Color color) {
  static constexpr std::array<SquareOffset, 2> kWhiteOffsets = {
      {{1, -1}, {-1, -1}}};
  static constexpr std::array<SquareOffset, 2> kBlackOffsets = {
      {{1, 1}, {-1, 1}}};

  co_yield std::ranges::elements_of(
      ((color == Color::kWhite) ? kWhiteOffsets : kBlackOffsets) |
      validMoves(sq));
}

inline std::generator<Square> pawnReverseSlidingMove(Square sq, Color color) {
  if (auto source = farthestPawnPushSource(sq, color)) {
    co_yield std::ranges::elements_of(
        traversedSquares(sq, *source, Origin::kSkip));
  }
}

inline std::generator<Square> pawnSlidingMove(Square sq, Color color) {
  if (auto destination = farthestPawnPush(sq, color)) {
    co_yield std::ranges::elements_of(
        traversedSquares(sq, *destination, Origin::kSkip));
  }
}

inline std::generator<Square> kingMoves(Square sq) {
  static constexpr std::array<SquareOffset, 8> kOffsets = {
      {{1, 1}, {1, 0}, {1, -1}, {0, 1}, {0, -1}, {-1, 1}, {-1, 0}, {-1, -1}}};

  co_yield std::ranges::elements_of(kOffsets | validMoves(sq));
}

inline std::generator<std::generator<Square>> rookSlidingMoves(Square sq) {
  co_yield (traversedSquares(sq, {sq.file, Rank::k1}, Origin::kSkip));
  co_yield (traversedSquares(sq, {sq.file, Rank::k8}, Origin::kSkip));
  co_yield (traversedSquares(sq, {File::kA, sq.rank}, Origin::kSkip));
  co_yield (traversedSquares(sq, {File::kH, sq.rank}, Origin::kSkip));
}

inline std::generator<std::generator<Square>> bishopSlidingMoves(Square sq) {
  constexpr static std::array<Square, 4> corners = {{{File::kA, Rank::k8},
                                                     {File::kH, Rank::k8},
                                                     {File::kA, Rank::k1},
                                                     {File::kH, Rank::k1}}};

  for (const auto& corner : corners) {
    auto projection = diagonalProjection(sq, corner);
    co_yield (traversedSquares(sq, projection, Origin::kSkip));
  }
}

inline std::generator<std::generator<Square>> queenSlidingMoves(Square sq) {
  co_yield std::ranges::elements_of(rookSlidingMoves(sq));
  co_yield std::ranges::elements_of(bishopSlidingMoves(sq));
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_MOVEGEN_INTERNAL_SQUARE_MOVEGEN_H_
