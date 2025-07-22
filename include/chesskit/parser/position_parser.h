#ifndef CHESSKIT_INCLUDE_CHESSKIT_PARSER_POSITION_PARSER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_PARSER_POSITION_PARSER_H_

// IWYU pragma: private, include "../position.h"

#include <charconv>
#include <cstdint>
#include <expected>
#include <system_error>

#include "../castling_rights.h"
#include "../color.h"
#include "../core/position.h"
#include "../parse_error.h"
#include "../piece_placement.h"
#include "../square.h"
#include "internal/try_parse_from.h"
#include "parse.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesskit {

namespace internal {

inline auto parseNumber(const char* begin, const char* end)
    -> std::expected<ParseResult<uint32_t, const char*>, ParseError> {
  uint32_t value{};
  auto [ptr, ec] = std::from_chars(begin, end, value);

  if (ec == std::errc()) return ParseResult{.parsed_value = value, .ptr = ptr};

  if (ec == std::errc::result_out_of_range) {
    return std::unexpected(ParseError::kNumberOutOfRange);
  }

  return std::unexpected(ParseError::kInvalidNumber);
}

inline auto parsePositionParams(const char* begin, const char* end)
    -> std::expected<ParseResult<Position::Params, const char*>, ParseError> {
  auto is_space = [&end](const auto& ptr) { return ptr != end && *ptr == ' '; };
  auto is_dash = [&end](const auto& ptr) { return ptr != end && *ptr == '-'; };

  const auto* ptr = begin;

  auto piece_placement = parseFrom<PiecePlacement>(ptr, end);
  if (!piece_placement) return std::unexpected(piece_placement.error());
  ptr = piece_placement->ptr;

  if (!is_space(ptr)) return std::unexpected(ParseError::kInvalidWhiteSpace);
  std::advance(ptr, 1);

  auto active_color = parseFrom<Color>(ptr, end);
  if (!active_color) return std::unexpected(active_color.error());
  ptr = active_color->ptr;

  if (!is_space(ptr)) return std::unexpected(ParseError::kInvalidWhiteSpace);
  std::advance(ptr, 1);

  auto castling_rights = parseFrom<CastlingRights>(ptr, end);
  if (!castling_rights) return std::unexpected(castling_rights.error());
  ptr = castling_rights->ptr;

  if (!is_space(ptr)) return std::unexpected(ParseError::kInvalidWhiteSpace);
  std::advance(ptr, 1);

  auto en_passant_square = internal::tryParseFrom<Square>(ptr, end);
  ptr = en_passant_square.ptr;
  if (!en_passant_square.parsed_value) {
    if (!is_dash(ptr)) return std::unexpected(ParseError::kInvalidDashSymbol);
    std::advance(ptr, 1);
  }

  if (!is_space(ptr)) return std::unexpected(ParseError::kInvalidWhiteSpace);
  std::advance(ptr, 1);

  auto halfmove_clock = parseNumber(ptr, end);
  if (!halfmove_clock) return std::unexpected(halfmove_clock.error());
  ptr = halfmove_clock->ptr;

  if (!is_space(ptr)) return std::unexpected(ParseError::kInvalidWhiteSpace);
  std::advance(ptr, 1);

  auto fullmove_number = parseNumber(ptr, end);
  if (!fullmove_number) return std::unexpected(fullmove_number.error());
  ptr = fullmove_number->ptr;

  auto params = Position::Params{
      .piece_placement = piece_placement->parsed_value,
      .active_color = active_color->parsed_value,
      .castling_rights = castling_rights->parsed_value,
      .en_passant_target_square = en_passant_square.parsed_value,
      .halfmove_clock = halfmove_clock->parsed_value,
      .fullmove_number = fullmove_number->parsed_value,
  };

  return ParseResult(params, ptr);
}

}  // namespace internal

/// @ingroup PositionHelpers
/// @brief parsing support for Position
template <>
class Parser<Position, const char*, parse_as::Default> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<Position, const char*>, ParseError> {
    auto params = internal::parsePositionParams(begin, end);
    if (!params) return std::unexpected(params.error());

    auto position = Position::fromParams(params->parsed_value);
    if (!position) return std::unexpected(ParseError::kInvalidPosition);

    return ParseResult(position.value(), params->ptr);
  }
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_PARSER_POSITION_PARSER_H_
