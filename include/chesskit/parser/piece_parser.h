#ifndef CHESSKIT_INCLUDE_CHESSKIT_PARSER_PIECE_PARSER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_PARSER_PIECE_PARSER_H_

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

namespace chesskit {

/// @ingroup PieceHelpers
/// @brief parsing support for Piece
template <>
class Parser<Piece, const char*, parse_as::Default> {
 public:
  constexpr std::expected<ParseResult<Piece, const char*>, ParseError> parse(
      const char* begin, const char* end) {
    auto whitePiece = parseFrom<PieceType>(begin, end, parse_as::Uppercase{});
    if (whitePiece)
      return ParseResult{Piece{whitePiece->parsedValue, Color::kWhite},
                         whitePiece->ptr};

    auto blackPiece = parseFrom<PieceType>(begin, end, parse_as::Lowercase{});
    if (blackPiece)
      return ParseResult{Piece{blackPiece->parsedValue, Color::kBlack},
                         blackPiece->ptr};

    return std::unexpected(ParseError::kInvalidPiece);
  }
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_PARSER_PIECE_PARSER_H_
