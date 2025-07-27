#ifndef CHESSCXX_INCLUDE_CHESSCXX_PARSER_SAN_MOVE_PARSER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_PARSER_SAN_MOVE_PARSER_H_

// IWYU pragma: private, include "../san_move.h"

#include <expected>
#include <iterator>
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

namespace chesscxx {

/// @ingroup SanMoveHelpers
/// @brief parsing support for SanNormalMove
template <>
class Parser<SanNormalMove, const char*, parse_as::Default> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<SanNormalMove, const char*>, ParseError> {
    const auto* ptr = begin;
    SanNormalMove move;

    auto piece =
        internal::tryParseFrom<PieceType>(ptr, end, parse_as::Uppercase{});
    move.piece_type = piece.parsed_value.value_or(PieceType::kPawn);
    ptr = piece.ptr;

    auto origin = parseFrom<PartialSquare>(ptr, end);
    if (!origin) return std::unexpected(origin.error());
    move.origin = origin->parsed_value;
    ptr = origin->ptr;

    move.is_capture = false;
    if (ptr != end && *ptr == 'x') {
      move.is_capture = true;
      std::advance(ptr, 1);
    }

    auto destination = parseFrom<Square>(ptr, end);
    if (destination) {
      move.destination = destination->parsed_value;
      ptr = destination->ptr;
    } else if (move.is_capture) {
      return std::unexpected(destination.error());
    } else {
      auto square_from_origin = internal::toSquare(move.origin);
      if (square_from_origin) {
        move.origin = PartialSquare();
        move.destination = *square_from_origin;
      } else {
        return std::unexpected(destination.error());
      }
    }

    bool has_promotion_symbol = false;
    if (ptr != end && *ptr == '=') {
      has_promotion_symbol = true;
      std::advance(ptr, 1);
    }

    auto promotion =
        parseFrom<PromotablePieceType>(ptr, end, parse_as::Uppercase{});
    if (promotion) {
      move.promotion = promotion->parsed_value;
      ptr = promotion->ptr;
    } else if (has_promotion_symbol) {
      return std::unexpected(promotion.error());
    }

    auto check_indicator = internal::tryParseFrom<CheckIndicator>(ptr, end);
    move.check_indicator = check_indicator.parsed_value;
    ptr = check_indicator.ptr;

    return ParseResult{.parsed_value = move, .ptr = ptr};
  }
};

/**
 *  @ingroup SanMoveHelpers
 *  @brief parsing support for SanCastlingMove
 */
template <>
class Parser<SanCastlingMove, const char*, parse_as::Default> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<SanCastlingMove, const char*>, ParseError> {
    static constexpr std::string_view kQueenside = "O-O-O";
    static constexpr std::string_view kKingside = "O-O";

    CastlingSide side{};
    const auto* ptr = begin;
    std::string_view const input{begin, end};

    if (input.starts_with(kQueenside)) {
      std::advance(ptr, kQueenside.size());
      side = CastlingSide::kQueenside;
    } else if (input.starts_with(kKingside)) {
      std::advance(ptr, kKingside.size());
      side = CastlingSide::kKingside;
    } else {
      return std::unexpected(ParseError::kInvalidSanCastling);
    }

    auto check_indicator = internal::tryParseFrom<CheckIndicator>(ptr, end);
    ptr = check_indicator.ptr;

    return ParseResult{
        .parsed_value =
            SanCastlingMove{.side = side,
                            .check_indicator = check_indicator.parsed_value},
        .ptr = ptr};
  }
};

/**
 *  @ingroup SanMoveHelpers
 *  @brief parsing support for SanMove
 */
template <>
class Parser<SanMove, const char*, parse_as::Default> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<SanMove, const char*>, ParseError> {
    auto castling = parseFrom<SanCastlingMove>(begin, end);
    if (castling) {
      return ParseResult<SanMove, const char*>{
          .parsed_value = castling->parsed_value, .ptr = castling->ptr};
    }

    auto normal = parseFrom<SanNormalMove>(begin, end);
    if (normal) {
      return ParseResult<SanMove, const char*>{
          .parsed_value = normal->parsed_value, .ptr = normal->ptr};
    }

    return std::unexpected(normal.error());
  }
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_PARSER_SAN_MOVE_PARSER_H_
