#ifndef CHESSCXX_INCLUDE_CHESSCXX_PARSER_GAME_RESULT_PARSER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_PARSER_GAME_RESULT_PARSER_H_

// IWYU pragma: private, include "../game_result.h"

#include <array>
#include <cstddef>
#include <expected>
#include <iterator>
#include <string_view>
#include <utility>

#include "../core/game_result.h"
#include "../parse_error.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesscxx {

/// @ingroup GameResultHelpers
/// @brief parsing support for GameResult
template <>
class Parser<GameResult, const char*, parse_as::Default> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<GameResult, const char*>, ParseError> {
    constexpr static std::array<std::pair<std::string_view, GameResult>, 3>
        kGameResults = {{{"1-0", GameResult::kWhiteWins},
                         {"0-1", GameResult::kBlackWins},
                         {"1/2-1/2", GameResult::kDraw}}};

    std::string_view const str(begin, end);

    for (const auto& [pattern, result] : kGameResults) {
      if (str.starts_with(pattern)) {
        return ParseResult{
            .parsed_value = result,
            .ptr = std::next(begin, static_cast<ptrdiff_t>(pattern.size()))};
      }
    }

    return std::unexpected(ParseError::kInvalidGameResult);
  }
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_PARSER_GAME_RESULT_PARSER_H_
