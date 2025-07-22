#ifndef CHESSKIT_INCLUDE_CHESSKIT_PARSER_INTERNAL_ENUM_PARSER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_PARSER_INTERNAL_ENUM_PARSER_H_

#include <expected>
#include <iterator>
#include <string_view>

#include "../../parse_error.h"
#include "../parse_result.h"

namespace chesskit::internal {

template <typename Enum>
constexpr auto parseEnum(const char* begin, const char* end,
                         std::string_view allowed, ParseError error)
    -> std::expected<ParseResult<Enum, const char*>, ParseError> {
  if (begin == end) return std::unexpected(error);

  auto index = allowed.find(*begin);

  if (index == std::string_view::npos) return std::unexpected(error);

  return ParseResult{static_cast<Enum>(index), std::next(begin)};
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_PARSER_INTERNAL_ENUM_PARSER_H_
