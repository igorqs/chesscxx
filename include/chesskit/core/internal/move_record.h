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
  CastlingRights previous_castling_rights;
  std::optional<File> previous_en_passant_file;
};

struct NormalMoveRecord {
  PieceType piece_type;
  PartialSquare partial_origin;
  std::optional<PieceType> captured_piece_type;
  bool is_en_passant_capture = false;
  UciMove uci_move;
  std::optional<CheckIndicator> check_indicator;
  std::optional<CastlingRights> previous_castling_rights;
  std::optional<File> previous_en_passant_file;
  uint32_t previous_halfmove_clock;
};

using MoveRecord = std::variant<NormalMoveRecord, CastlingMoveRecord>;

template <typename MoveOutput>
struct Converter;

template <>
struct Converter<UciMove> {
  auto operator()(const NormalMoveRecord& move) -> UciMove {
    return move.uci_move;
  }
  auto operator()(const CastlingMoveRecord& move) -> UciMove {
    auto raw_move = castlingMoves(move.side, move.color).king_move;
    return UciMove(raw_move.origin, raw_move.destination, std::nullopt);
  }
};

template <>
struct Converter<SanMove> {
  auto operator()(const NormalMoveRecord& move) -> SanMove {
    return SanNormalMove{.piece_type = move.piece_type,
                         .origin = move.partial_origin,
                         .is_capture = move.captured_piece_type.has_value() ||
                                       move.is_en_passant_capture,
                         .destination = move.uci_move.destination,
                         .promotion = move.uci_move.promotion,
                         .check_indicator = move.check_indicator};
  }
  auto operator()(const CastlingMoveRecord& move) -> SanMove {
    return SanCastlingMove(move.side);
  }
};

template <typename MovedOutput>
constexpr auto convertTo(const MoveRecord& move_record) -> MovedOutput {
  return std::visit(Converter<MovedOutput>{}, move_record);
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_MOVE_RECORD_H_
