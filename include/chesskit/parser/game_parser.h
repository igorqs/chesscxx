#ifndef CHESSKIT_INCLUDE_CHESSKIT_PARSER_GAME_PARSER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_PARSER_GAME_PARSER_H_

// IWYU pragma: private, include "../game.h"

#include <algorithm>
#include <cctype>
#include <expected>
#include <optional>
#include <string_view>

#include "../core/game.h"
#include "../game_result.h"
#include "../parse_error.h"
#include "../position.h"
#include "../san_move.h"
#include "parse.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesskit {
/// @ingroup GameHelpers
/// @brief FEN parsing support for Game
template <>
class Parser<Game, const char*, parse_as::Fen> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<Game, const char*>, ParseError> {
    auto position = parseFrom<Position>(begin, end);
    if (position) {
      return ParseResult{.parsed_value = Game(position->parsed_value),
                         .ptr = position->ptr};
    }

    return std::unexpected(position.error());
  }
};

/// @ingroup GameHelpers
/// @brief PGN parsing support for Game
template <>
class Parser<Game, const char*, parse_as::Pgn> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<Game, const char*>, ParseError> {
    const auto* ptr = begin;

    auto position = parseTags(ptr, end);
    if (!position) return std::unexpected(position.error());
    ptr = position->ptr;

    auto game = parseMoveList(ptr, end, position->parsed_value);
    if (!game) return std::unexpected(game.error());
    ptr = game->ptr;

    return ParseResult{.parsed_value = game->parsed_value, .ptr = ptr};
  }

 private:
  static auto parseTags(const char* begin, const char* end)
      -> std::expected<ParseResult<std::optional<Position>, const char*>,
                       ParseError> {
    auto is_right_bracket = [](const char& input) { return input == ']'; };

    const auto* ptr = begin;
    std::optional<Position> position;

    while (ptr < end) {
      ptr = trimAnySpaces(ptr, end);

      if (ptr == end || !isLeftBracket(*ptr)) {
        return ParseResult{.parsed_value = position, .ptr = ptr};
      }
      ++ptr;

      auto parsed_tag = parseTag(ptr, end);
      if (!parsed_tag) return std::unexpected(parsed_tag.error());
      if (parsed_tag->parsed_value.has_value()) {
        if (position.has_value()) {
          return std::unexpected(ParseError::kDuplicatedFenTag);
        }
        position = parsed_tag->parsed_value;
      }
      ptr = parsed_tag->ptr;

      if (ptr == end || !is_right_bracket(*ptr)) {
        return std::unexpected(ParseError::kInvalidRightBracket);
      }
      ++ptr;
    }

    return ParseResult{.parsed_value = position, .ptr = ptr};
  }

  static auto parseTag(const char* begin, const char* end)
      -> std::expected<ParseResult<std::optional<Position>, const char*>,
                       ParseError> {
    const auto* ptr = begin;
    std::optional<Position> position;

    ptr = trimWhiteSpaces(ptr, end);

    if (ptr == end || !isSymbolToken(*ptr)) {
      return std::unexpected(ParseError::kInvalidTag);
    }

    auto str = std::string_view(ptr, end);
    bool const is_fen = str.starts_with("FEN") &&
                        (ptr + 3 == end || !isSymbolToken(*(ptr + 3)));

    ptr = trimSymbolToken(ptr, end);

    ptr = trimWhiteSpaces(ptr, end);

    if (ptr == end || !isQuote(*ptr)) {
      return std::unexpected(ParseError::kInvalidQuote);
    }
    ptr++;

    if (is_fen) {
      auto parsed_position = parseFrom<Position>(ptr, end);
      if (!parsed_position) return std::unexpected(parsed_position.error());
      ptr = parsed_position->ptr;
      position = parsed_position->parsed_value;
    } else {
      ptr = trimStringToken(ptr, end);
    }

    if (ptr == end || !isQuote(*ptr)) {
      return std::unexpected(ParseError::kInvalidQuote);
    }
    ptr++;

    ptr = trimWhiteSpaces(ptr, end);

    return ParseResult{.parsed_value = position, .ptr = ptr};
  }

  static auto parseMoveList(const char* begin, const char* end,
                            const std::optional<Position>& position)
      -> std::expected<ParseResult<Game, const char*>, ParseError> {
    auto is_asterisk = [](const char& input) { return input == '*'; };

    Game game = position.has_value() ? Game(*position) : Game();
    const auto* ptr = begin;

    while (ptr != end) {
      ptr = trimAnySkippableToken(ptr, end);

      // Game termination marks
      if (ptr == end) break;
      if (is_asterisk(*ptr)) {
        ptr++;
        break;
      }
      auto game_result = parseFrom<GameResult>(ptr, end);
      if (game_result) {
        ptr = game_result->ptr;
        break;
      }
      if (isLeftBracket(*ptr)) break;

      // San Move
      auto parsed_move = parseFrom<SanMove>(ptr, end);
      if (!parsed_move) return std::unexpected(parsed_move.error());

      const auto& move = parsed_move->parsed_value;
      ptr = parsed_move->ptr;

      if (!game.move(move)) return std::unexpected(ParseError::kInvalidMove);
    }

    return ParseResult{.parsed_value = game, .ptr = ptr};
  }

  static auto trimAnySkippableToken(const char* begin, const char* end) -> const
      char* {
    const auto* ptr = begin;

    while (ptr != end) {
      const auto* skipped = trimAnySpaces(ptr, end);
      skipped = trimBlockComment(skipped, end);
      skipped = trimRav(skipped, end);
      skipped = trimLineComment(skipped, end);
      skipped = trimNag(skipped, end);
      skipped = trimPeriod(skipped, end);
      skipped = trimMoveAnnotation(skipped, end);
      skipped = trimDigit(skipped, end);

      if (skipped == ptr) break;
      ptr = skipped;
    }

    return ptr;
  }

  static auto trimWhiteSpaces(const char* begin, const char* end) -> const
      char* {
    auto is_white_space = [](const char& input) { return input == ' '; };

    return std::find_if_not(begin, end, is_white_space);
  }

  static auto trimAnySpaces(const char* begin, const char* end) -> const char* {
    return std::find_if_not(begin, end, isAnySpace);
  }

  static auto trimBlock(const char* begin, const char* end,
                        std::array<char, 2> block_delimiters) -> const char* {
    if (begin != end && *begin == block_delimiters[0]) {
      begin = std::find(begin + 1, end, block_delimiters[1]);
      if (begin != end) begin++;
    }

    return begin;
  }

  static auto trimBlockComment(const char* begin, const char* end) -> const
      char* {
    return trimBlock(begin, end, {'{', '}'});
  }

  static auto trimRav(const char* begin, const char* end) -> const char* {
    return trimBlock(begin, end, {'(', ')'});
  }

  static auto trimLineComment(const char* begin, const char* end) -> const
      char* {
    if (begin != end && isSemicolon(*begin)) {
      begin = std::find(begin + 1, end, '\n');
    }

    return begin;
  }

  static auto trimNag(const char* begin, const char* end) -> const char* {
    if (begin != end && isDollarSign(*begin)) {
      begin = std::find_if_not(begin + 1, end, isDigit);
    }

    return begin;
  }

  static auto trimPeriod(const char* begin, const char* end) -> const char* {
    if (begin != end && isPeriod(*begin)) begin++;

    return begin;
  }

  static auto trimMoveAnnotation(const char* begin, const char* end) -> const
      char* {
    if (begin != end && isMoveAnnotation(*begin)) begin++;

    return begin;
  }

  static auto trimDigit(const char* begin, const char* end) -> const char* {
    if (begin != end && isDigit(*begin)) {
      const auto* skipped = std::find_if_not(begin + 1, end, isDigit);

      bool const was_number = (skipped == end) || isSkippable(*skipped);

      if (was_number) return skipped;
    }

    return begin;
  }

  static auto isSkippable(char input) -> bool {
    return isLeftBrace(input) || isLeftParenthesis(input) ||
           isSemicolon(input) || isDollarSign(input) || isPeriod(input) ||
           isMoveAnnotation(input) || isDigit(input) || isAnySpace(input);
  };

  static auto trimSymbolToken(const char* begin, const char* end) -> const
      char* {
    return std::find_if_not(begin, end, isSymbolToken);
  }

  static auto trimStringToken(const char* begin, const char* end) -> const
      char* {
    auto is_string_escape = [](const char& input) { return input == '\\'; };

    const auto* ptr = begin;

    bool escaping = false;

    for (; ptr != end; ++ptr) {
      if (is_string_escape(*ptr)) {
        escaping = !escaping;
        continue;
      }

      if (isQuote(*ptr) && !escaping) break;

      escaping = false;
    }

    return ptr;
  }

  static auto isAlpha(char input) -> bool { return std::isalpha(input) != 0; };
  static auto isDigit(char input) -> bool { return std::isdigit(input) != 0; };
  static auto isAnySpace(char input) -> bool {
    return std::isspace(input) != 0;
  };
  static auto isLeftBrace(char input) -> bool { return input == '{'; };
  static auto isLeftParenthesis(char input) -> bool { return input == '('; };
  static auto isSemicolon(char input) -> bool { return input == ';'; };
  static auto isDollarSign(char input) -> bool { return input == '$'; };
  static auto isPeriod(char input) -> bool { return input == '.'; };
  static auto isMoveAnnotation(char input) -> bool {
    return input == '!' || input == '?';
  };
  static auto isLeftBracket(char input) -> bool { return input == '['; };
  static auto isQuote(char input) -> bool { return input == '\"'; };
  static auto isSymbolToken(char input) -> bool {
    constexpr static std::string_view kValid = "_+#=:-";
    return (kValid.contains(input) || isAlpha(input) || isDigit(input));
  };
};

/// @ingroup GameHelpers
/// @brief default parsing support for Game
template <>
class Parser<Game, const char*, parse_as::Default>
    : public Parser<Game, const char*, parse_as::Pgn> {};
}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_PARSER_GAME_PARSER_H_
