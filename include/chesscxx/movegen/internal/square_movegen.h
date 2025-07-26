#ifndef CHESSCXX_INCLUDE_CHESSCXX_MOVEGEN_INTERNAL_SQUARE_MOVEGEN_H_
#define CHESSCXX_INCLUDE_CHESSCXX_MOVEGEN_INTERNAL_SQUARE_MOVEGEN_H_

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

namespace chesscxx::internal {

enum class Origin : uint8_t {
  kKeep,
  kSkip,
};

inline auto validMoves(Square square) {
  return std::views::transform(
             [square](auto offset) { return shiftSquare(square, offset); }) |
         std::views::filter(
             [](auto optional) { return optional.has_value(); }) |
         std::views::transform([](auto optional) { return *optional; });
}

inline auto diagonalProjection(const Square& square, const Square& corner)
    -> Square {
  SquareOffset offset = calculateOffset(corner, square);
  bool const same_sign =
      std::signbit(offset.file_offset) == std::signbit(offset.rank_offset);
  int8_t const multiplier = same_sign ? 1 : -1;

  if (std::abs(offset.file_offset) < std::abs(offset.rank_offset)) {
    offset.rank_offset = offset.file_offset * multiplier;
  } else {
    offset.file_offset = offset.rank_offset * multiplier;
  }

  auto projection = shiftSquare(square, offset);
  if (projection) return *projection;

  std::unreachable();
}

inline auto isOrthogonal(const SquareOffset& offset) -> bool {
  return offset.file_offset == 0 || offset.rank_offset == 0;
}

inline auto isDiagonal(const SquareOffset& offset) -> bool {
  return std::abs(offset.file_offset) == std::abs(offset.rank_offset);
}

inline auto isStraightLine(const SquareOffset& offset) -> bool {
  return isOrthogonal(offset) || isDiagonal(offset);
}

inline auto slidingIncrement(const SquareOffset& offset)
    -> std::optional<SquareOffset> {
  if (!isStraightLine(offset)) return std::nullopt;

  static constexpr int kLo = -1;
  static constexpr int kHi = 1;

  return SquareOffset{.file_offset = std::clamp(offset.file_offset, kLo, kHi),
                      .rank_offset = std::clamp(offset.rank_offset, kLo, kHi)};
}

inline auto traversedSquares(Square origin, Square destination,
                             Origin origin_policy = Origin::kKeep)
    -> std::generator<Square> {
  if (origin_policy == Origin::kSkip && origin == destination) co_return;

  auto increment = calculateOffset(destination, origin);

  if (const auto& sliding_inc = slidingIncrement(increment)) {
    increment = *sliding_inc;
  }

  std::optional<Square> square = origin;
  if (origin_policy == Origin::kSkip) square = shiftSquare(*square, increment);

  for (; square && *square != destination;
       square = shiftSquare(*square, increment)) {
    co_yield *square;
  }

  co_yield destination;
}

inline auto fileNeighbors(Square square) -> std::generator<Square> {
  static constexpr std::array<SquareOffset, 2> kOffsets = {
      {{.file_offset = 1, .rank_offset = 0},
       {.file_offset = -1, .rank_offset = 0}}};

  co_yield std::ranges::elements_of(kOffsets | validMoves(square));
}

inline auto knightMoves(Square square) -> std::generator<Square> {
  static constexpr std::array<SquareOffset, 8> kOffsets = {
      {{.file_offset = 2, .rank_offset = 1},
       {.file_offset = 2, .rank_offset = -1},
       {.file_offset = 1, .rank_offset = 2},
       {.file_offset = 1, .rank_offset = -2},
       {.file_offset = -1, .rank_offset = 2},
       {.file_offset = -1, .rank_offset = -2},
       {.file_offset = -2, .rank_offset = 1},
       {.file_offset = -2, .rank_offset = -1}}};

  co_yield std::ranges::elements_of(kOffsets | validMoves(square));
}

inline auto pawnCaptures(Square square, Color color) -> std::generator<Square> {
  static constexpr std::array<SquareOffset, 2> kWhiteOffsets = {
      {{.file_offset = 1, .rank_offset = -1},
       {.file_offset = -1, .rank_offset = -1}}};
  static constexpr std::array<SquareOffset, 2> kBlackOffsets = {
      {{.file_offset = 1, .rank_offset = 1},
       {.file_offset = -1, .rank_offset = 1}}};

  co_yield std::ranges::elements_of(
      ((color == Color::kWhite) ? kWhiteOffsets : kBlackOffsets) |
      validMoves(square));
}

inline auto pawnReverseSlidingMove(Square square, Color color)
    -> std::generator<Square> {
  if (auto source = farthestPawnPushSource(square, color)) {
    co_yield std::ranges::elements_of(
        traversedSquares(square, *source, Origin::kSkip));
  }
}

inline auto pawnSlidingMove(Square square, Color color)
    -> std::generator<Square> {
  if (auto destination = farthestPawnPush(square, color)) {
    co_yield std::ranges::elements_of(
        traversedSquares(square, *destination, Origin::kSkip));
  }
}

inline auto kingMoves(Square square) -> std::generator<Square> {
  static constexpr std::array<SquareOffset, 8> kOffsets = {
      {{.file_offset = 1, .rank_offset = 1},
       {.file_offset = 1, .rank_offset = 0},
       {.file_offset = 1, .rank_offset = -1},
       {.file_offset = 0, .rank_offset = 1},
       {.file_offset = 0, .rank_offset = -1},
       {.file_offset = -1, .rank_offset = 1},
       {.file_offset = -1, .rank_offset = 0},
       {.file_offset = -1, .rank_offset = -1}}};

  co_yield std::ranges::elements_of(kOffsets | validMoves(square));
}

inline auto rookSlidingMoves(Square square)
    -> std::generator<std::generator<Square>> {
  co_yield (traversedSquares(square, {.file = square.file, .rank = Rank::k1},
                             Origin::kSkip));
  co_yield (traversedSquares(square, {.file = square.file, .rank = Rank::k8},
                             Origin::kSkip));
  co_yield (traversedSquares(square, {.file = File::kA, .rank = square.rank},
                             Origin::kSkip));
  co_yield (traversedSquares(square, {.file = File::kH, .rank = square.rank},
                             Origin::kSkip));
}

inline auto bishopSlidingMoves(Square square)
    -> std::generator<std::generator<Square>> {
  constexpr static std::array<Square, 4> kCorners = {
      {{.file = File::kA, .rank = Rank::k8},
       {.file = File::kH, .rank = Rank::k8},
       {.file = File::kA, .rank = Rank::k1},
       {.file = File::kH, .rank = Rank::k1}}};

  for (const auto& corner : kCorners) {
    auto projection = diagonalProjection(square, corner);
    co_yield (traversedSquares(square, projection, Origin::kSkip));
  }
}

inline auto queenSlidingMoves(Square square)
    -> std::generator<std::generator<Square>> {
  co_yield std::ranges::elements_of(rookSlidingMoves(square));
  co_yield std::ranges::elements_of(bishopSlidingMoves(square));
}

}  // namespace chesscxx::internal

#endif  // CHESSCXX_INCLUDE_CHESSCXX_MOVEGEN_INTERNAL_SQUARE_MOVEGEN_H_
