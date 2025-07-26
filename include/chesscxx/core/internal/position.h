#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_POSITION_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_POSITION_H_

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

namespace chesscxx::internal {

constexpr auto isCheck(const Position& position) -> bool {
  return isKingAttacked(position.piecePlacement(), position.activeColor());
}
constexpr auto isCheckmate(const Position& position) -> bool {
  return !hasLegalMove(position) &&
         isKingAttacked(position.piecePlacement(), position.activeColor());
}
constexpr auto isStalemate(const Position& position) -> bool {
  return !hasLegalMove(position) &&
         !isKingAttacked(position.piecePlacement(), position.activeColor());
}
constexpr auto isFiftyMoveRuleDraw(const Position& position) -> bool {
  constexpr static auto kFiftyMoveLimit = 100;
  return position.halfmoveClock() >= kFiftyMoveLimit && hasLegalMove(position);
}
constexpr auto isInsufficientMaterialDraw(const Position& position) -> bool {
  return isInsufficientMaterialDraw(position.piecePlacement());
}
constexpr auto isDraw(const Position& position) -> bool {
  return isStalemate(position) || isFiftyMoveRuleDraw(position) ||
         isInsufficientMaterialDraw(position);
}
constexpr auto isGameOver(const Position& position) -> bool {
  return isCheckmate(position) || isDraw(position);
}

inline auto uciFromSan(const Position& position, const SanNormalMove& san_move,
                       const Color& color)
    -> std::expected<UciMove, MoveError> {
  auto piece_type = san_move.piece_type;
  auto destination = san_move.destination;

  auto possible_origins = piecesReaching(position, destination,
                                         {.type = piece_type, .color = color});
  auto partial_origin = san_move.origin;
  std::optional<Square> origin;

  for (const Square& square : possible_origins) {
    if (!matches(partial_origin, square)) continue;

    if (!origin) {
      origin = square;
    } else {
      return std::unexpected(MoveError::kAmbiguousOrigin);
    }
  }

  if (!origin) return std::unexpected(MoveError::kNoValidOrigin);

  return UciMove(*origin, destination, san_move.promotion);
}

inline auto partialOriginFromMove(const Position& position, const RawMove& move)
    -> std::expected<PartialSquare, MoveError> {
  const auto& origin = move.origin;
  const auto& destination = move.destination;

  auto piece = pieceAt(position.piecePlacement(), origin);
  if (!piece) return std::unexpected(MoveError::kNoPieceAtOrigin);

  if (piece->type == PieceType::kPawn) {
    auto captured_piece = pieceAt(position.piecePlacement(), destination);
    bool const is_en_passant_capture =
        position.enPassantTargetSquare() == destination;
    if (!captured_piece && !is_en_passant_capture) return PartialSquare{};
    return PartialSquare(origin.file, std::nullopt);
  }

  auto possible_origins = piecesReaching(position, destination, *piece);

  std::unordered_map<File, uint8_t> file_counter;
  std::unordered_map<Rank, uint8_t> rank_counter;

  int total = 0;
  for (const Square& square : possible_origins) {
    file_counter[square.file]++;
    rank_counter[square.rank]++;
    total++;
  }

  if (total == 0) return std::unexpected(MoveError::kNoValidOrigin);
  if (total == 1) return PartialSquare{};

  if (file_counter[origin.file] == 1) {
    return PartialSquare(origin.file, std::nullopt);
  }

  if (rank_counter[origin.rank] == 1) {
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

  auto possible_origins = piecesReaching(position, destination, *piece);

  if (std::ranges::find(possible_origins, origin) == possible_origins.end()) {
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

  auto destination_piece = pieceAt(position.piecePlacement(), move.destination);
  auto origin_piece = pieceAt(position.piecePlacement(), move.origin);
  if (!origin_piece) return MoveError::kNoPieceAtOrigin;
  bool const is_pawn_move = origin_piece->type == PieceType::kPawn;
  bool const is_normal_capture = destination_piece.has_value();

  if (!is_pawn_move && !is_normal_capture &&
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

}  // namespace chesscxx::internal

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_POSITION_H_
