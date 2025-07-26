#ifndef CHESSCXX_INCLUDE_CHESSCXX_PARSER_PIECE_PARSER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_PARSER_PIECE_PARSER_H_

// IWYU pragma: private, include "../piece_type.h"

#include <expected>

#include "../core/color.h"
#include "../core/piece.h"
#include "../core/piece_type.h"
#include "../parse_error.h"
#include "parse.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesscxx {

/// @ingroup PieceHelpers
/// @brief parsing support for Piece
template <>
class Parser<Piece, const char*, parse_as::Default> {
 public:
  static constexpr auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<Piece, const char*>, ParseError> {
    auto white_piece = parseFrom<PieceType>(begin, end, parse_as::Uppercase{});
    if (white_piece) {
      return ParseResult{
          .parsed_value =
              Piece{.type = white_piece->parsed_value, .color = Color::kWhite},
          .ptr = white_piece->ptr};
    }

    auto black_piece = parseFrom<PieceType>(begin, end, parse_as::Lowercase{});
    if (black_piece) {
      return ParseResult{
          .parsed_value =
              Piece{.type = black_piece->parsed_value, .color = Color::kBlack},
          .ptr = black_piece->ptr};
    }

    return std::unexpected(ParseError::kInvalidPiece);
  }
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_PARSER_PIECE_PARSER_H_
