#ifndef CHESSCXX_INCLUDE_CHESSCXX_PARSER_UCI_MOVE_PARSER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_PARSER_UCI_MOVE_PARSER_H_

// IWYU pragma: private, include "../uci_move.h"

#include <expected>

#include "../core/uci_move.h"
#include "../parse_error.h"
#include "../piece_type.h"
#include "../square.h"
#include "internal/try_parse_from.h"
#include "parse.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesscxx {

/// @ingroup UciMoveHelpers
/// @brief parsing support for UciMove
template <>
class Parser<UciMove, const char*, parse_as::Default> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<UciMove, const char*>, ParseError> {
    auto origin = parseFrom<Square>(begin, end);
    if (!origin) return std::unexpected(origin.error());

    auto destination = parseFrom<Square>(origin->ptr, end);
    if (!destination) return std::unexpected(destination.error());

    auto promotion = internal::tryParseFrom<PromotablePieceType>(
        destination->ptr, end, parse_as::Lowercase{});

    auto uci = UciMove(origin->parsed_value, destination->parsed_value,
                       promotion.parsed_value);

    return ParseResult{.parsed_value = uci, .ptr = promotion.ptr};
  }
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_PARSER_UCI_MOVE_PARSER_H_
