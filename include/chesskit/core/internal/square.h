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
  int8_t fileOffset;
  int8_t rankOffset;
};

inline constexpr std::optional<Square> createSquarefromIndex(
    const uint8_t& index) {
  uint8_t rankIndex = index / kNumFiles;
  uint8_t fileIndex = index % kNumFiles;
  auto file = toFile(fileIndex);
  auto rank = toRank(rankIndex);

  if (!file || !rank) return std::nullopt;
  return Square(*file, *rank);
}

inline constexpr SquareOffset calculateOffset(const Square& lhs,
                                              const Square& rhs) {
  return {static_cast<int8_t>(index(lhs.file) - index(rhs.file)),
          static_cast<int8_t>(index(lhs.rank) - index(rhs.rank))};
}

inline constexpr std::optional<Square> shiftSquare(const Square& sq,
                                                   const SquareOffset& offset) {
  auto newFile = toFile(index(sq.file) + offset.fileOffset);
  auto newRank = toRank(index(sq.rank) + offset.rankOffset);

  if (!newFile || !newRank) return std::nullopt;

  return Square(*newFile, *newRank);
}

inline constexpr std::optional<Square> advanceRanks(const Square& sq,
                                                    int8_t ranks,
                                                    const Color& color) {
  if (color == Color::kWhite) ranks = -ranks;

  return shiftSquare(sq, SquareOffset{0, ranks});
}

inline constexpr std::optional<Square> squareBehind(const Square& sq,
                                                    int8_t ranks,
                                                    const Color& color) {
  return advanceRanks(sq, -ranks, color);
}

inline constexpr std::optional<Square> squareAhead(const Square& sq,
                                                   int8_t ranks,
                                                   const Color& color) {
  return advanceRanks(sq, ranks, color);
}

inline constexpr std::optional<Square> farthestPawnPushSource(
    const Square& sq, const Color& color) {
  return isDoublePawnPushTargetRank(sq.rank, color)
             ? squareBehind(sq, 2, color)
             : squareBehind(sq, 1, color);
}

inline constexpr std::optional<Square> farthestPawnPush(const Square& sq,
                                                        const Color& color) {
  return isPawnStartingRank(sq.rank, color) ? squareAhead(sq, 2, color)
                                            : squareAhead(sq, 1, color);
}

inline constexpr Color square_color(const Square& sq) {
  auto parity = (index(sq.file) + index(sq.rank)) & 1;
  return (parity == 0) ? Color::kWhite : Color::kBlack;
}

inline constexpr Square backRankSquare(const File& file, const Color& color) {
  return Square(file, backRank(color));
}

inline constexpr std::optional<File> toOptionalFile(
    const std::optional<Square>& square) {
  if (!square) return std::nullopt;

  return std::optional<File>(square->file);
}

inline bool isValidEnPassantTargetSquare(const std::optional<Square>& square,
                                         const Color& color) {
  if (!square) return true;

  return square->rank == enPassantRank(color);
}

inline auto enPassantCapturedPawnSquare(const Square& enPassantTargetSquare,
                                        const Color& attacker_color) {
  return internal::squareBehind(enPassantTargetSquare, 1, attacker_color);
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_SQUARE_H_
