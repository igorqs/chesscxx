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

std::expected<ParseResult<uint32_t, const char*>, ParseError> parseNumber(
    const char* begin, const char* end) {
  uint32_t value{};
  auto [ptr, ec] = std::from_chars(begin, end, value);

  if (ec == std::errc()) return ParseResult{value, ptr};

  if (ec == std::errc::result_out_of_range)
    return std::unexpected(ParseError::kNumberOutOfRange);

  return std::unexpected(ParseError::kInvalidNumber);
}

std::expected<ParseResult<Position::Params, const char*>, ParseError>
parsePositionParams(const char* begin, const char* end) {
  auto isSpace = [&end](const auto& it) { return it != end && *it == ' '; };
  auto isDash = [&end](const auto& it) { return it != end && *it == '-'; };

  auto ptr = begin;

  auto piecePlacement = parseFrom<PiecePlacement>(ptr, end);
  if (!piecePlacement) return std::unexpected(piecePlacement.error());
  ptr = piecePlacement->ptr;

  if (!isSpace(ptr++)) return std::unexpected(ParseError::kInvalidWhiteSpace);

  auto activeColor = parseFrom<Color>(ptr, end);
  if (!activeColor) return std::unexpected(activeColor.error());
  ptr = activeColor->ptr;

  if (!isSpace(ptr++)) return std::unexpected(ParseError::kInvalidWhiteSpace);

  auto castlingRights = parseFrom<CastlingRights>(ptr, end);
  if (!castlingRights) return std::unexpected(castlingRights.error());
  ptr = castlingRights->ptr;

  if (!isSpace(ptr++)) return std::unexpected(ParseError::kInvalidWhiteSpace);

  auto enPassantSquare = internal::tryParseFrom<Square>(ptr, end);
  ptr = enPassantSquare.ptr;
  if (!enPassantSquare.parsedValue) {
    if (!isDash(ptr++)) return std::unexpected(ParseError::kInvalidDashSymbol);
  }

  if (!isSpace(ptr++)) return std::unexpected(ParseError::kInvalidWhiteSpace);

  auto halfmoveClock = parseNumber(ptr, end);
  if (!halfmoveClock) return std::unexpected(halfmoveClock.error());
  ptr = halfmoveClock->ptr;

  if (!isSpace(ptr++)) return std::unexpected(ParseError::kInvalidWhiteSpace);

  auto fullmoveNumber = parseNumber(ptr, end);
  if (!fullmoveNumber) return std::unexpected(fullmoveNumber.error());
  ptr = fullmoveNumber->ptr;

  auto params = Position::Params{
      .piecePlacement = piecePlacement->parsedValue,
      .activeColor = activeColor->parsedValue,
      .castlingRights = castlingRights->parsedValue,
      .enPassantTargetSquare = enPassantSquare.parsedValue,
      .halfmoveClock = halfmoveClock->parsedValue,
      .fullmoveNumber = fullmoveNumber->parsedValue,
  };

  return ParseResult(params, ptr);
}

}  // namespace internal

/// @ingroup PositionHelpers
/// @brief parsing support for Position
template <>
class Parser<Position, const char*, parse_as::Default> {
 public:
  std::expected<ParseResult<Position, const char*>, ParseError> parse(
      const char* begin, const char* end) {
    auto params = internal::parsePositionParams(begin, end);
    if (!params) return std::unexpected(params.error());

    auto position = Position::fromParams(params->parsedValue);
    if (!position) return std::unexpected(ParseError::kInvalidPosition);

    return ParseResult(position.value(), params->ptr);
  }
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_PARSER_POSITION_PARSER_H_
