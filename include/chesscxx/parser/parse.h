#ifndef CHESSCXX_INCLUDE_CHESSCXX_PARSER_PARSE_H_
#define CHESSCXX_INCLUDE_CHESSCXX_PARSER_PARSE_H_

#include <expected>
#include <string_view>

#include "../parse_error.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesscxx {

/// @brief Parses an object of type T from a range of input iterators.
///
/// @tparam T The type to be parsed.
/// @tparam InputIt Iterator type pointing to the input range.
/// @tparam Tag Optional parsing tag to customize parsing behavior. Defaults to
/// parse_as::Default.
///
/// @param begin Iterator to the beginning of the input range.
/// @param end Iterator to the end of the input range.
/// @param tag Parsing tag to control specific parsing rules.
///
/// @return A `std::expected` containing a ParseResult<T, InputIt> on success,
/// or a ParseError on failure.
/// - On success: `parsedValue` holds the parsed T, and `ptr` points to the
/// first character not processed.
/// - On failure: ParseError indicating the failure reason.
///
/// @note This function requires a specialization of Parser<T, InputIt, Tag> to
/// be defined. If no such specialization exists, the function is ill-formed and
/// will not compile.
template <typename T, typename InputIt, typename Tag = parse_as::Default>
constexpr auto parseFrom(InputIt begin, InputIt end,
                         Tag /*tag*/ = parse_as::Default{})
    -> std::expected<ParseResult<T, InputIt>, ParseError> {
  return Parser<T, InputIt, Tag>{}.parse(begin, end);
}

/// @brief Parses an object of type T from a string view, expecting the entire
/// string to be consumed.
///
/// @tparam T The type to be parsed.
/// @tparam Tag Optional parsing tag to customize parsing behavior. Defaults to
/// parse_as::Default.
///
/// @param str The string view to parse.
/// @param tag Parsing tag to control specific parsing rules.
///
/// @return A std::expected containing the parsed value on success, or a
/// ParseError on failure. Fails with ParseError::kExpectingEndOfString if any
/// unparsed characters remain.
///
/// @note This function requires a specialization of Parser<T, const char*, Tag>
/// to be defined. If no such specialization exists, the function is ill-formed
/// and will not compile.
template <typename T, typename Tag = parse_as::Default>
constexpr auto parse(std::string_view str, Tag tag = parse_as::Default{})
    -> std::expected<T, ParseError> {
  using InputIt = std::string_view::const_pointer;

  auto parse_result = parseFrom<T, InputIt, Tag>(str.begin(), str.end(), tag);

  if (!parse_result) return std::unexpected(parse_result.error());

  if (parse_result->ptr != str.end()) {
    return std::unexpected(ParseError::kExpectingEndOfString);
  }

  return parse_result->parsed_value;
}

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_PARSER_PARSE_H_
