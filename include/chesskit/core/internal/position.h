#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_POSITION_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_POSITION_H_

#include <algorithm>
#include <cstdint>
#include <expected>
#include <optional>
#include <unordered_map>

#include "../../color.h"
#include "../../file.h"
#include "../../move_error.h"
#include "../../movegen/internal/position_movegen.h"
#include "../../piece_type.h"
#include "../../position.h"
#include "../../rank.h"
#include "../../san_move.h"
#include "../../square.h"
#include "../../uci_move.h"
#include "partial_square.h"
#include "piece_placement.h"
#include "piece_placement_material.h"
#include "piece_placement_piece_at.h"
#include "raw_move.h"

namespace chesskit::internal {

inline constexpr auto isCheck(const Position& position) -> bool {
  return isKingAttacked(position.piecePlacement(), position.activeColor());
}
inline constexpr auto isCheckmate(const Position& position) -> bool {
  return !hasLegalMove(position) &&
         isKingAttacked(position.piecePlacement(), position.activeColor());
}
inline constexpr auto isStalemate(const Position& position) -> bool {
  return !hasLegalMove(position) &&
         !isKingAttacked(position.piecePlacement(), position.activeColor());
}
inline constexpr auto isFiftyMoveRuleDraw(const Position& position) -> bool {
  return position.halfmoveClock() >= 100 && hasLegalMove(position);
}
inline constexpr auto isInsufficientMaterialDraw(const Position& position)
    -> bool {
  return isInsufficientMaterialDraw(position.piecePlacement());
}
inline constexpr auto isDraw(const Position& position) -> bool {
  return isStalemate(position) || isFiftyMoveRuleDraw(position) ||
         isInsufficientMaterialDraw(position);
}
inline constexpr auto isGameOver(const Position& position) -> bool {
  return isCheckmate(position) || isDraw(position);
}

inline auto uciFromSan(const Position& position, const SanNormalMove& sanMove,
                       const Color& color)
    -> std::expected<UciMove, MoveError> {
  auto pieceType = sanMove.pieceType;
  auto destination = sanMove.destination;

  auto possibleOrigins = piecesReaching(position, destination,
                                        {.type = pieceType, .color = color});
  auto partialOrigin = sanMove.origin;
  std::optional<Square> origin;

  for (const Square& square : possibleOrigins) {
    if (!matches(partialOrigin, square)) continue;

    if (!origin) {
      origin = square;
    } else {
      return std::unexpected(MoveError::kAmbiguousOrigin);
    }
  }

  if (!origin) return std::unexpected(MoveError::kNoValidOrigin);

  return UciMove(*origin, destination, sanMove.promotion);
}

inline auto partialOriginFromMove(const Position& position, const RawMove& move)
    -> std::expected<PartialSquare, MoveError> {
  const auto& origin = move.origin;
  const auto& destination = move.destination;

  auto piece = pieceAt(position.piecePlacement(), origin);
  if (!piece) return std::unexpected(MoveError::kNoPieceAtOrigin);

  if (piece->type == PieceType::kPawn) {
    auto capturedPiece = pieceAt(position.piecePlacement(), destination);
    bool const isEnPassantCapture =
        position.enPassantTargetSquare() == destination;
    if (!capturedPiece && !isEnPassantCapture) return PartialSquare{};
    return PartialSquare(origin.file, std::nullopt);
  }

  auto possibleOrigins = piecesReaching(position, destination, *piece);

  std::unordered_map<File, uint8_t> fileCounter;
  std::unordered_map<Rank, uint8_t> rankCounter;

  int total = 0;
  for (const Square& square : possibleOrigins) {
    fileCounter[square.file]++;
    rankCounter[square.rank]++;
    total++;
  }

  if (total == 0) return std::unexpected(MoveError::kNoValidOrigin);
  if (total == 1) return PartialSquare{};

  if (fileCounter[origin.file] == 1) {
    return PartialSquare(origin.file, std::nullopt);
  }

  if (rankCounter[origin.rank] == 1) {
    return PartialSquare(std::nullopt, origin.rank);
  }

  return PartialSquare(origin.file, origin.rank);
}

inline auto uciMoveError(const Position& position, const RawMove& move)
    -> std::optional<MoveError> {
  const auto& origin = move.origin;
  const auto& destination = move.destination;

  auto piece = pieceAt(position.piecePlacement(), origin);
  if (!piece) return MoveError::kNoPieceAtOrigin;

  auto possibleOrigins = piecesReaching(position, destination, *piece);

  if (std::ranges::find(possibleOrigins, origin) == possibleOrigins.end()) {
    return MoveError::kIllegalMove;
  }

  return std::nullopt;
}

inline auto overflowError(const Position& position, const RawMove& move)
    -> std::optional<MoveError> {
  if (position.activeColor() == Color::kBlack &&
      position.fullmoveNumber() == Position::kMaxFullmoveNumber) {
    return MoveError::kFullmoveNumberOverflow;
  }

  auto destinationPiece = pieceAt(position.piecePlacement(), move.destination);
  auto originPiece = pieceAt(position.piecePlacement(), move.origin);
  if (!originPiece) return MoveError::kNoPieceAtOrigin;
  bool const isPawnMove = originPiece->type == PieceType::kPawn;
  bool const isNormalCapture = destinationPiece.has_value();

  if (!isPawnMove && !isNormalCapture &&
      position.halfmoveClock() == Position::kMaxHalfmoveClock) {
    return MoveError::kHalfmoveClockOverflow;
  }

  return std::nullopt;
}

inline auto overflowError(const Position& position)
    -> std::optional<MoveError> {
  if (position.activeColor() == Color::kBlack &&
      position.fullmoveNumber() == Position::kMaxFullmoveNumber) {
    return MoveError::kFullmoveNumberOverflow;
  }

  if (position.halfmoveClock() == Position::kMaxHalfmoveClock) {
    return MoveError::kHalfmoveClockOverflow;
  }

  return std::nullopt;
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_POSITION_H_
