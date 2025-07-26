#ifndef CHESSCXX_INCLUDE_CHESSCXX_PARSER_RANK_PARSER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_PARSER_RANK_PARSER_H_

// IWYU pragma: private, include "../rank.h"

#include <expected>
#include <string_view>

#include "../core/rank.h"
#include "../parse_error.h"
#include "internal/enum_parser.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesscxx {

/// @ingroup RankHelpers
/// @brief parsing support for Rank
template <>
class Parser<Rank, const char*, parse_as::Default> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<Rank, const char*>, ParseError> {
    static constexpr std::string_view kAllowed = "87654321";

    return internal::parseEnum<Rank>(begin, end, kAllowed,
                                     ParseError::kInvalidRank);
  }
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_PARSER_RANK_PARSER_H_
