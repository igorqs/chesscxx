#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_MOVE_RECORD_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_MOVE_RECORD_H_

#include <cstdint>
#include <optional>
#include <variant>

#include "../../castling_rights.h"
#include "../../castling_side.h"
#include "../../check_indicator.h"
#include "../../color.h"
#include "../../file.h"
#include "../../partial_square.h"
#include "../../piece_type.h"
#include "../../san_move.h"
#include "../../uci_move.h"
#include "castling_rules.h"

namespace chesskit::internal {

struct CastlingMoveRecord {
  CastlingSide side;
  Color color;
  std::optional<CheckIndicator> check_indicator;
  CastlingRights previousCastlingRights;
  std::optional<File> previousEnPassantFile;
};

struct NormalMoveRecord {
  PieceType pieceType;
  PartialSquare partialOrigin;
  std::optional<PieceType> capturedPieceType;
  bool isEnPassantCapture = false;
  UciMove uciMove;
  std::optional<CheckIndicator> check_indicator;
  std::optional<CastlingRights> previousCastlingRights;
  std::optional<File> previousEnPassantFile;
  uint32_t previousHalfmoveClock;
};

using MoveRecord = std::variant<NormalMoveRecord, CastlingMoveRecord>;

template <typename MoveOutput>
struct Converter;

template <>
struct Converter<UciMove> {
  auto operator()(const NormalMoveRecord& move) -> UciMove {
    return move.uciMove;
  };
  auto operator()(const CastlingMoveRecord& move) -> UciMove {
    auto rawMove = castlingMoves(move.side, move.color).kingMove;
    return UciMove(rawMove.origin, rawMove.destination, std::nullopt);
  };
};

template <>
struct Converter<SanMove> {
  auto operator()(const NormalMoveRecord& move) -> SanMove {
    return SanNormalMove{.pieceType = move.pieceType,
                         .origin = move.partialOrigin,
                         .isCapture = move.capturedPieceType.has_value() ||
                                      move.isEnPassantCapture,
                         .destination = move.uciMove.destination,
                         .promotion = move.uciMove.promotion,
                         .check_indicator = move.check_indicator};
  };
  auto operator()(const CastlingMoveRecord& move) -> SanMove {
    return SanCastlingMove(move.side);
  };
};

template <typename MovedOutput>
inline constexpr auto convertTo(const MoveRecord& moveRecord) -> MovedOutput {
  return std::visit(Converter<MovedOutput>{}, moveRecord);
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_MOVE_RECORD_H_
