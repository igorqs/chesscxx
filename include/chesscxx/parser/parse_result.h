#ifndef CHESSCXX_INCLUDE_CHESSCXX_PARSER_PARSER_RESULT_H_
#define CHESSCXX_INCLUDE_CHESSCXX_PARSER_PARSER_RESULT_H_

namespace chesscxx {

/// @brief Result of a parsing operation.
///
/// @tparam T The type of the parsed value.
/// @tparam InputIt The input iterator type.
template <typename T, typename InputIt>
struct ParseResult {
  /// The value obtained from parsing the input.
  T parsed_value;
  /// Iterator pointing to the remaining unparsed input.
  InputIt ptr;
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_PARSER_PARSER_RESULT_H_
