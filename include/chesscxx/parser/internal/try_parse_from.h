#ifndef CHESSCXX_INCLUDE_CHESSCXX_PARSER_TRY_PARSE_FROM_H_
#define CHESSCXX_INCLUDE_CHESSCXX_PARSER_TRY_PARSE_FROM_H_

#include <optional>

#include "../parse.h"
#include "../parse_result.h"
#include "../parse_tags.h"

namespace chesscxx::internal {

template <typename T, typename InputIt, typename Tag = parse_as::Default>
constexpr auto tryParseFrom(InputIt begin, InputIt end,
                            Tag tag = parse_as::Default{})
    -> ParseResult<std::optional<T>, InputIt> {
  auto result = parseFrom<T, InputIt, Tag>(begin, end, tag);
  if (result) {
    return ParseResult<std::optional<T>, InputIt>{result->parsed_value,
                                                  result->ptr};
  }

  return ParseResult<std::optional<T>, InputIt>{std::nullopt, begin};
}

}  // namespace chesscxx::internal

#endif  // CHESSCXX_INCLUDE_CHESSCXX_PARSER_TRY_PARSE_FROM_H_
