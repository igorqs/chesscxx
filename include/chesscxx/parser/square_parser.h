#ifndef CHESSCXX_INCLUDE_CHESSCXX_PARSER_SQUARE_PARSER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_PARSER_SQUARE_PARSER_H_

// IWYU pragma: private, include "../square.h"

#include <expected>

#include "../core/square.h"
#include "../file.h"
#include "../parse_error.h"
#include "../rank.h"
#include "parse.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesscxx {

/// @ingroup SquareHelpers
/// @brief parsing support for Square
template <>
class Parser<Square, const char*, parse_as::Default> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<Square, const char*>, ParseError> {
    auto file = parseFrom<File>(begin, end);
    if (!file) return std::unexpected(file.error());

    auto rank = parseFrom<Rank>(file->ptr, end);
    if (!rank) return std::unexpected(rank.error());

    auto square = Square(file->parsed_value, rank->parsed_value);

    return ParseResult{.parsed_value = square, .ptr = rank->ptr};
  }
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_PARSER_SQUARE_PARSER_H_
