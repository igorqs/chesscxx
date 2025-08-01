#ifndef CHESSCXX_INCLUDE_CHESSCXX_PARSER_CHECK_INDICATOR_PARSER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_PARSER_CHECK_INDICATOR_PARSER_H_

// IWYU pragma: private, include "../check_indicator.h"

#include <expected>
#include <string_view>

#include "../core/check_indicator.h"
#include "../parse_error.h"
#include "internal/enum_parser.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesscxx {

/// @ingroup CheckIndicatorHelpers
/// @brief parsing support for CheckIndicator
template <>
class Parser<CheckIndicator, const char*, parse_as::Default> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<CheckIndicator, const char*>, ParseError> {
    static constexpr std::string_view kAllowed = "+#";

    return internal::parseEnum<CheckIndicator>(
        begin, end, kAllowed, ParseError::kInvalidCheckIndicator);
  }
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_PARSER_CHECK_INDICATOR_PARSER_H_
