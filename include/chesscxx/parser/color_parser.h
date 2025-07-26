#ifndef CHESSCXX_INCLUDE_CHESSCXX_PARSER_COLOR_PARSER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_PARSER_COLOR_PARSER_H_

// IWYU pragma: private, include "../color.h"

#include <expected>
#include <string_view>

#include "../core/color.h"
#include "../parse_error.h"
#include "internal/enum_parser.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesscxx {

/// @ingroup ColorHelpers
/// @brief parsing support for Color
template <>
class Parser<Color, const char*, parse_as::Default> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<Color, const char*>, ParseError> {
    static constexpr std::string_view kAllowed = "wb";

    return internal::parseEnum<Color>(begin, end, kAllowed,
                                      ParseError::kInvalidColor);
  }
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_PARSER_COLOR_PARSER_H_
