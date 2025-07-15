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
  auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<Game, const char*>, ParseError> {
    auto position = parseFrom<Position>(begin, end);
    if (position) {
      return ParseResult{.parsedValue = Game(position->parsedValue),
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
  auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<Game, const char*>, ParseError> {
    auto ptr = begin;

    auto position = parseTags(ptr, end);
    if (!position) return std::unexpected(position.error());
    ptr = position->ptr;

    auto game = parseMoveList(ptr, end, position->parsedValue);
    if (!game) return std::unexpected(game.error());
    ptr = game->ptr;

    return ParseResult{.parsedValue = game->parsedValue, .ptr = ptr};
  }

 private:
  auto parseTags(const char* begin, const char* end)
      -> std::expected<ParseResult<std::optional<Position>, const char*>,
                       ParseError> {
    auto isRightBracket = [](const char& c) { return c == ']'; };

    auto ptr = begin;
    std::optional<Position> position;

    while (ptr < end) {
      ptr = trimAnySpaces(ptr, end);

      if (ptr == end || !isLeftBracket(*ptr)) {
        return ParseResult{.parsedValue = position, .ptr = ptr};
      }
      ++ptr;

      auto parsedTag = parseTag(ptr, end);
      if (!parsedTag) return std::unexpected(parsedTag.error());
      if (parsedTag->parsedValue.has_value()) {
        if (position.has_value()) {
          return std::unexpected(ParseError::kDuplicatedFenTag);
        }
        position = parsedTag->parsedValue;
      }
      ptr = parsedTag->ptr;

      if (ptr == end || !isRightBracket(*ptr)) {
        return std::unexpected(ParseError::kInvalidRightBracket);
      }
      ++ptr;
    }

    return ParseResult{.parsedValue = position, .ptr = ptr};
  }

  auto parseTag(const char* begin, const char* end)
      -> std::expected<ParseResult<std::optional<Position>, const char*>,
                       ParseError> {
    auto ptr = begin;
    std::optional<Position> position;

    ptr = trimWhiteSpaces(ptr, end);

    if (ptr == end || !isSymbolToken(*ptr)) {
      return std::unexpected(ParseError::kInvalidTag);
    }

    auto sv = std::string_view(ptr, end);
    bool const isFen =
        sv.starts_with("FEN") && (ptr + 3 == end || !isSymbolToken(*(ptr + 3)));

    ptr = trimSymbolToken(ptr, end);

    ptr = trimWhiteSpaces(ptr, end);

    if (ptr == end || !isQuote(*ptr)) {
      return std::unexpected(ParseError::kInvalidQuote);
    }
    ptr++;

    if (isFen) {
      auto parsedPosition = parseFrom<Position>(ptr, end);
      if (!parsedPosition) return std::unexpected(parsedPosition.error());
      ptr = parsedPosition->ptr;
      position = parsedPosition->parsedValue;
    } else {
      ptr = trimStringToken(ptr, end);
    }

    if (ptr == end || !isQuote(*ptr)) {
      return std::unexpected(ParseError::kInvalidQuote);
    }
    ptr++;

    ptr = trimWhiteSpaces(ptr, end);

    return ParseResult{.parsedValue = position, .ptr = ptr};
  }

  auto parseMoveList(const char* begin, const char* end,
                     const std::optional<Position>& position)
      -> std::expected<ParseResult<Game, const char*>, ParseError> {
    auto isAsterisk = [](const char& c) { return c == '*'; };

    Game game = position.has_value() ? Game(*position) : Game();
    auto ptr = begin;

    while (ptr != end) {
      ptr = trimAnySkippableToken(ptr, end);

      // Game termination marks
      if (ptr == end) break;
      if (isAsterisk(*ptr)) {
        ptr++;
        break;
      }
      auto gameResult = parseFrom<GameResult>(ptr, end);
      if (gameResult) {
        ptr = gameResult->ptr;
        break;
      }
      if (isLeftBracket(*ptr)) break;

      // San Move
      auto parsedMove = parseFrom<SanMove>(ptr, end);
      if (!parsedMove) return std::unexpected(parsedMove.error());

      const auto& move = parsedMove->parsedValue;
      ptr = parsedMove->ptr;

      if (!game.move(move)) return std::unexpected(ParseError::kInvalidMove);
    }

    return ParseResult{.parsedValue = game, .ptr = ptr};
  }

  auto trimAnySkippableToken(const char* begin, const char* end) -> const
      char* {
    auto isLeftBrace = [](const char& c) { return c == '{'; };
    auto isLeftParenthesis = [](const char& c) { return c == '('; };
    auto isSemicolon = [](const char& c) { return c == ';'; };
    auto isDollarSign = [](const char& c) { return c == '$'; };
    auto isPeriod = [](const char& c) { return c == '.'; };
    auto isMoveAnnotation = [](const char& c) { return c == '!' || c == '?'; };
    auto isDigit = [](const char& c) { return std::isdigit(c); };

    auto isSkippable = [&](const char& c) {
      return isLeftBrace(c) || isLeftParenthesis(c) || isSemicolon(c) ||
             isDollarSign(c) || isPeriod(c) || isMoveAnnotation(c) ||
             isDigit(c) || isAnySpace(c);
    };

    auto ptr = begin;

    while (ptr != end) {
      ptr = trimAnySpaces(ptr, end);

      if (ptr == end) break;

      if (isLeftBrace(*ptr)) {
        ptr = std::find(ptr + 1, end, '}');
        if (ptr != end) ptr++;
        continue;
      }

      if (isLeftParenthesis(*ptr)) {
        ptr = std::find(ptr + 1, end, ')');
        if (ptr != end) ptr++;
        continue;
      }

      if (isSemicolon(*ptr)) {
        ptr = std::find(ptr + 1, end, '\n');
        continue;
      }

      if (isDollarSign(*ptr)) {
        ptr = std::find_if_not(ptr + 1, end, isDigit);
        continue;
      }

      if (isPeriod(*ptr)) {
        ptr++;
        continue;
      }

      if (isMoveAnnotation(*ptr)) {
        ptr++;
        continue;
      }

      if (isDigit(*ptr)) {
        auto skipped = std::find_if_not(ptr + 1, end, isDigit);

        bool const wasNumber = (skipped == end) || isSkippable(*skipped);

        if (!wasNumber) break;
        ptr = skipped;
        continue;
      }

      break;
    }

    return ptr;
  }

  auto trimWhiteSpaces(const char* begin, const char* end) -> const char* {
    auto isWhiteSpace = [](const char& c) { return c == ' '; };

    return std::find_if_not(begin, end, isWhiteSpace);
  }

  auto trimAnySpaces(const char* begin, const char* end) -> const char* {
    return std::find_if_not(begin, end, isAnySpace);
  }

  auto trimSymbolToken(const char* begin, const char* end) -> const char* {
    return std::find_if_not(begin, end, isSymbolToken);
  }

  auto trimStringToken(const char* begin, const char* end) -> const char* {
    auto isStringEscape = [](const char& c) { return c == '\\'; };

    auto ptr = begin;

    bool escaping = false;

    for (; ptr != end; ++ptr) {
      if (isStringEscape(*ptr)) {
        escaping = !escaping;
        continue;
      }

      if (isQuote(*ptr) && !escaping) break;

      escaping = false;
    }

    return ptr;
  }

  static auto isAnySpace(const char& c) -> bool { return std::isspace(c); };

  static auto isLeftBracket(const char& c) -> bool { return c == '['; };

  static auto isQuote(const char& c) -> bool { return c == '\"'; };

  static auto isSymbolToken(const char& c) -> bool {
    constexpr static std::string_view valid = "_+#=:-";
    return (valid.contains(c) || std::isalpha(c) || std::isdigit(c));
  };
};

/// @ingroup GameHelpers
/// @brief default parsing support for Game
template <>
class Parser<Game, const char*, parse_as::Default>
    : public Parser<Game, const char*, parse_as::Pgn> {};
}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_PARSER_GAME_PARSER_H_
