#ifndef CHESSKIT_INCLUDE_CHESSKIT_PARSER_SAN_MOVE_PARSER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_PARSER_SAN_MOVE_PARSER_H_

// IWYU pragma: private, include "../san_move.h"

#include <expected>
#include <optional>
#include <string_view>

#include "../castling_side.h"
#include "../check_indicator.h"
#include "../core/internal/partial_square.h"
#include "../core/san_move.h"
#include "../parse_error.h"
#include "../partial_square.h"
#include "../piece_type.h"
#include "../square.h"
#include "internal/try_parse_from.h"
#include "parse.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesskit {

/// @ingroup SanMoveHelpers
/// @brief parsing support for SanNormalMove
template <>
class Parser<SanNormalMove, const char*, parse_as::Default> {
 public:
  std::expected<ParseResult<SanNormalMove, const char*>, ParseError> parse(
      const char* begin, const char* end) {
    auto ptr = begin;
    SanNormalMove move;

    auto piece =
        internal::tryParseFrom<PieceType>(ptr, end, parse_as::Uppercase{});
    move.pieceType = piece.parsedValue.value_or(PieceType::kPawn);
    ptr = piece.ptr;

    auto origin = parseFrom<PartialSquare>(ptr, end);
    if (!origin) return std::unexpected(origin.error());
    move.origin = origin->parsedValue;
    ptr = origin->ptr;

    move.isCapture = false;
    if (ptr != end && *ptr == 'x') {
      move.isCapture = true;
      ptr++;
    }

    auto destination = parseFrom<Square>(ptr, end);
    if (destination) {
      move.destination = destination->parsedValue;
      ptr = destination->ptr;
    } else if (move.isCapture) {
      return std::unexpected(destination.error());
    } else {
      auto squareFromOrigin = internal::toSquare(move.origin);
      if (squareFromOrigin) {
        move.origin = PartialSquare();
        move.destination = *squareFromOrigin;
      } else {
        return std::unexpected(destination.error());
      }
    }

    bool hasPromotionSymbol = false;
    if (ptr != end && *ptr == '=') {
      hasPromotionSymbol = true;
      ptr++;
    }

    auto promotion =
        parseFrom<PromotablePieceType>(ptr, end, parse_as::Uppercase{});
    if (promotion) {
      move.promotion = promotion->parsedValue;
      ptr = promotion->ptr;
    } else if (hasPromotionSymbol) {
      return std::unexpected(promotion.error());
    }

    auto check_indicator = internal::tryParseFrom<CheckIndicator>(ptr, end);
    move.check_indicator = check_indicator.parsedValue;
    ptr = check_indicator.ptr;

    return ParseResult{move, ptr};
  }
};

/**
 *  @ingroup SanMoveHelpers
 *  @brief parsing support for SanCastlingMove
 */
template <>
class Parser<SanCastlingMove, const char*, parse_as::Default> {
 public:
  std::expected<ParseResult<SanCastlingMove, const char*>, ParseError> parse(
      const char* begin, const char* end) {
    static constexpr std::string_view queenside = "O-O-O";
    static constexpr std::string_view kingside = "O-O";

    CastlingSide side;
    auto ptr = begin;
    std::string_view const input{begin, end};

    if (input.starts_with(queenside)) {
      ptr += queenside.size();
      side = CastlingSide::kQueenside;
    } else if (input.starts_with(kingside)) {
      ptr += kingside.size();
      side = CastlingSide::kKingside;
    } else {
      return std::unexpected(ParseError::kInvalidSanCastling);
    }

    auto check_indicator = internal::tryParseFrom<CheckIndicator>(ptr, end);
    ptr = check_indicator.ptr;

    return ParseResult{SanCastlingMove{side, check_indicator.parsedValue}, ptr};
  }
};

/**
 *  @ingroup SanMoveHelpers
 *  @brief parsing support for SanMove
 */
template <>
class Parser<SanMove, const char*, parse_as::Default> {
 public:
  std::expected<ParseResult<SanMove, const char*>, ParseError> parse(
      const char* begin, const char* end) {
    auto castling = parseFrom<SanCastlingMove>(begin, end);
    if (castling) {
      return ParseResult<SanMove, const char*>{castling->parsedValue,
                                               castling->ptr};
    }

    auto normal = parseFrom<SanNormalMove>(begin, end);
    if (normal) {
      return ParseResult<SanMove, const char*>{normal->parsedValue,
                                               normal->ptr};
    }

    return std::unexpected(normal.error());
  }
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_PARSER_SAN_MOVE_PARSER_H_
