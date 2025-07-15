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
  int fileOffset;
  int rankOffset;
};

inline constexpr auto createSquareFromIndex(const uint8_t& index)
    -> std::optional<Square> {
  uint8_t const rankIndex = index / kNumFiles;
  uint8_t const fileIndex = index % kNumFiles;
  auto file = toFile(fileIndex);
  auto rank = toRank(rankIndex);

  if (!file || !rank) return std::nullopt;
  return Square(*file, *rank);
}

inline constexpr auto calculateOffset(const Square& lhs, const Square& rhs)
    -> SquareOffset {
  return {.fileOffset = index(lhs.file) - index(rhs.file),
          .rankOffset = index(lhs.rank) - index(rhs.rank)};
}

inline constexpr auto shiftSquare(const Square& sq, const SquareOffset& offset)
    -> std::optional<Square> {
  auto newFile = toFile(index(sq.file) + offset.fileOffset);
  auto newRank = toRank(index(sq.rank) + offset.rankOffset);

  if (!newFile || !newRank) return std::nullopt;

  return Square(*newFile, *newRank);
}

inline constexpr auto advanceRanks(const Square& sq, int ranks,
                                   const Color& color)
    -> std::optional<Square> {
  if (color == Color::kWhite) ranks = -ranks;

  return shiftSquare(sq, SquareOffset{.fileOffset = 0, .rankOffset = ranks});
}

inline constexpr auto squareBehind(const Square& sq, int ranks,
                                   const Color& color)
    -> std::optional<Square> {
  return advanceRanks(sq, -ranks, color);
}

inline constexpr auto squareAhead(const Square& sq, int ranks,
                                  const Color& color) -> std::optional<Square> {
  return advanceRanks(sq, ranks, color);
}

inline constexpr auto farthestPawnPushSource(const Square& sq,
                                             const Color& color)
    -> std::optional<Square> {
  return isDoublePawnPushTargetRank(sq.rank, color)
             ? squareBehind(sq, 2, color)
             : squareBehind(sq, 1, color);
}

inline constexpr auto farthestPawnPush(const Square& sq, const Color& color)
    -> std::optional<Square> {
  return isPawnStartingRank(sq.rank, color) ? squareAhead(sq, 2, color)
                                            : squareAhead(sq, 1, color);
}

inline constexpr auto square_color(const Square& sq) -> Color {
  auto parity = (index(sq.file) + index(sq.rank)) & 1;
  return (parity == 0) ? Color::kWhite : Color::kBlack;
}

inline constexpr auto backRankSquare(const File& file, const Color& color)
    -> Square {
  return Square(file, backRank(color));
}

inline constexpr auto toOptionalFile(const std::optional<Square>& square)
    -> std::optional<File> {
  if (!square) return std::nullopt;

  return {square->file};
}

inline auto isValidEnPassantTargetSquare(const std::optional<Square>& square,
                                         const Color& color) -> bool {
  if (!square) return true;

  return square->rank == enPassantRank(color);
}

inline auto enPassantCapturedPawnSquare(const Square& enPassantTargetSquare,
                                        const Color& attacker_color) {
  return internal::squareBehind(enPassantTargetSquare, 1, attacker_color);
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_SQUARE_H_
