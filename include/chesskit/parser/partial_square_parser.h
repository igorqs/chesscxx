#ifndef CHESSKIT_INCLUDE_CHESSKIT_PARSER_PARTIAL_SQUARE_PARSER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_PARSER_PARTIAL_SQUARE_PARSER_H_

// IWYU pragma: private, include "../partial_square.h"

#include <expected>

#include "../core/partial_square.h"
#include "../file.h"
#include "../parse_error.h"
#include "../rank.h"
#include "internal/try_parse_from.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesskit {

/**
 *  @ingroup PartialSquareHelpers
 *  @brief parsing support for PartialSquare
 */
template <>
class Parser<PartialSquare, const char*, parse_as::Default> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<PartialSquare, const char*>, ParseError> {
    auto file = internal::tryParseFrom<File>(begin, end);
    auto rank = internal::tryParseFrom<Rank>(file.ptr, end);

    auto square = PartialSquare(file.parsed_value, rank.parsed_value);

    return ParseResult{.parsed_value = square, .ptr = rank.ptr};
  }
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_PARSER_PARTIAL_SQUARE_PARSER_H_
