#ifndef CHESSKIT_INCLUDE_CHESSKIT_PARSER_RANK_PARSER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_PARSER_RANK_PARSER_H_

// IWYU pragma: private, include "../rank.h"

#include <expected>
#include <string_view>

#include "../core/rank.h"
#include "../parse_error.h"
#include "internal/enum_parser.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesskit {

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

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_PARSER_RANK_PARSER_H_
