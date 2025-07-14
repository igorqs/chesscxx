#ifndef CHESSKIT_INCLUDE_CHESSKIT_PARSER_GAME_RESULT_PARSER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_PARSER_GAME_RESULT_PARSER_H_

// IWYU pragma: private, include "../game_result.h"

#include <array>
#include <expected>
#include <string_view>
#include <utility>

#include "../core/game_result.h"
#include "../parse_error.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesskit {

/// @ingroup GameResultHelpers
/// @brief parsing support for GameResult
template <>
class Parser<GameResult, const char*, parse_as::Default> {
 public:
  std::expected<ParseResult<GameResult, const char*>, ParseError> parse(
      const char* begin, const char* end) {
    constexpr static std::array<std::pair<std::string_view, GameResult>, 3>
        kGameResults = {{{"1-0", GameResult::kWhiteWins},
                         {"0-1", GameResult::kBlackWins},
                         {"1/2-1/2", GameResult::kDraw}}};

    std::string_view const sv(begin, end);

    for (const auto& [pattern, result] : kGameResults) {
      if (sv.starts_with(pattern)) {
        return ParseResult{result, begin + pattern.size()};
      }
    }

    return std::unexpected(ParseError::kInvalidGameResult);
  }
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_PARSER_GAME_RESULT_PARSER_H_
