#ifndef CHESSCXX_INCLUDE_CHESSCXX_PARSER_PARSER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_PARSER_PARSER_H_

namespace chesscxx {

/// @brief Defines how to parse objects of type T from an input range.
///
/// This template is not defined and must be specialized for each supported
/// combination of `T`, `InputIt`, and `Tag`.
///
/// @tparam T The type to be parsed.
/// @tparam InputIt The type of the input iterators.
/// @tparam Tag The tag to customize parsing behavior.
template <typename T, typename InputIt, typename Tag>
class Parser;

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_PARSER_PARSER_H_
