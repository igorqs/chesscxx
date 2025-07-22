#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_SQUARE_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_SQUARE_H_

#include <cstdint>
#include <optional>

#include "../../color.h"
#include "../../file.h"
#include "../../square.h"
#include "file.h"
#include "rank.h"

namespace chesskit::internal {
struct SquareOffset {
  int file_offset;
  int rank_offset;
};

constexpr auto createSquareFromIndex(const uint8_t& index)
    -> std::optional<Square> {
  uint8_t const rank_index = index / kNumFiles;
  uint8_t const file_index = index % kNumFiles;
  auto file = toFile(file_index);
  auto rank = toRank(rank_index);

  if (!file || !rank) return std::nullopt;
  return Square(*file, *rank);
}

constexpr auto calculateOffset(const Square& lhs, const Square& rhs)
    -> SquareOffset {
  return {.file_offset = index(lhs.file) - index(rhs.file),
          .rank_offset = index(lhs.rank) - index(rhs.rank)};
}

constexpr auto shiftSquare(const Square& square, const SquareOffset& offset)
    -> std::optional<Square> {
  auto new_file = toFile(index(square.file) + offset.file_offset);
  auto new_rank = toRank(index(square.rank) + offset.rank_offset);

  if (!new_file || !new_rank) return std::nullopt;

  return Square(*new_file, *new_rank);
}

constexpr auto advanceRanks(const Square& square, int ranks, const Color& color)
    -> std::optional<Square> {
  if (color == Color::kWhite) ranks = -ranks;

  return shiftSquare(square,
                     SquareOffset{.file_offset = 0, .rank_offset = ranks});
}

constexpr auto squareBehind(const Square& square, int ranks, const Color& color)
    -> std::optional<Square> {
  return advanceRanks(square, -ranks, color);
}

constexpr auto squareAhead(const Square& square, int ranks, const Color& color)
    -> std::optional<Square> {
  return advanceRanks(square, ranks, color);
}

constexpr auto farthestPawnPushSource(const Square& square, const Color& color)
    -> std::optional<Square> {
  return isDoublePawnPushTargetRank(square.rank, color)
             ? squareBehind(square, 2, color)
             : squareBehind(square, 1, color);
}

constexpr auto farthestPawnPush(const Square& square, const Color& color)
    -> std::optional<Square> {
  return isPawnStartingRank(square.rank, color) ? squareAhead(square, 2, color)
                                                : squareAhead(square, 1, color);
}

constexpr auto squareColor(const Square& square) -> Color {
  auto parity = static_cast<uint>(index(square.file) + index(square.rank)) & 1U;
  return (parity == 0) ? Color::kWhite : Color::kBlack;
}

constexpr auto backRankSquare(const File& file, const Color& color) -> Square {
  return Square(file, backRank(color));
}

constexpr auto toOptionalFile(const std::optional<Square>& square)
    -> std::optional<File> {
  if (!square) return std::nullopt;

  return {square->file};
}

inline auto isValidEnPassantTargetSquare(const std::optional<Square>& square,
                                         const Color& color) -> bool {
  if (!square) return true;

  return square->rank == enPassantRank(color);
}

inline auto enPassantCapturedPawnSquare(const Square& en_passant_target_square,
                                        const Color& attacker_color) {
  return internal::squareBehind(en_passant_target_square, 1, attacker_color);
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_SQUARE_H_
