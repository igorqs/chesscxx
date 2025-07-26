#ifndef CHESSCXX_INCLUDE_CHESSCXX_PARSER_FILE_PARSER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_PARSER_FILE_PARSER_H_

// IWYU pragma: private, include "../file.h"

#include <expected>
#include <string_view>

#include "../core/file.h"
#include "../parse_error.h"
#include "internal/enum_parser.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesscxx {

/// @ingroup FileHelpers
/// @brief parsing support for File
template <>
class Parser<File, const char*, parse_as::Default> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<File, const char*>, ParseError> {
    static constexpr std::string_view kAllowed = "abcdefgh";

    return internal::parseEnum<File>(begin, end, kAllowed,
                                     ParseError::kInvalidFile);
  }
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_PARSER_FILE_PARSER_H_
