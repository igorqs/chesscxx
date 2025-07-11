#ifndef CHESSKIT_INCLUDE_CHESSKIT_PARSER_PIECE_TYPE_PARSER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_PARSER_PIECE_TYPE_PARSER_H_

// IWYU pragma: private, include "../piece_type.h"

#include <expected>
#include <string_view>

#include "../core/piece_type.h"
#include "../parse_error.h"
#include "internal/enum_parser.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesskit {

/// @ingroup PieceTypeHelpers
/// @brief uppercase parsing support for PieceType
template <>
class Parser<PieceType, const char*, parse_as::Uppercase> {
 public:
  constexpr std::expected<ParseResult<PieceType, const char*>, ParseError>
  parse(const char* begin, const char* end) {
    static constexpr std::string_view allowed = "PNBRQK";

    return internal::parseEnum<PieceType>(begin, end, allowed,
                                          ParseError::kInvalidPieceType);
  }
};

/// @ingroup PieceTypeHelpers
/// @brief lowercase parsing support for PieceType
template <>
class Parser<PieceType, const char*, parse_as::Lowercase> {
 public:
  constexpr std::expected<ParseResult<PieceType, const char*>, ParseError>
  parse(const char* begin, const char* end) {
    static constexpr std::string_view allowed = "pnbrqk";

    return internal::parseEnum<PieceType>(begin, end, allowed,
                                          ParseError::kInvalidPieceType);
  }
};

/// @ingroup PieceTypeHelpers
/// @brief uppercase parsing support for PromotablePieceType
template <>
class Parser<PromotablePieceType, const char*, parse_as::Uppercase> {
 public:
  constexpr std::expected<ParseResult<PromotablePieceType, const char*>,
                          ParseError>
  parse(const char* begin, const char* end) {
    static constexpr std::string_view allowed = "NBRQ";

    return internal::parseEnum<PromotablePieceType>(
        begin, end, allowed, ParseError::kInvalidPromotablePieceType);
  }
};

/// @ingroup PieceTypeHelpers
/// @brief lowercase parsing support for PromotablePieceType
template <>
class Parser<PromotablePieceType, const char*, parse_as::Lowercase> {
 public:
  constexpr std::expected<ParseResult<PromotablePieceType, const char*>,
                          ParseError>
  parse(const char* begin, const char* end) {
    static constexpr std::string_view allowed = "nbrq";

    return internal::parseEnum<PromotablePieceType>(
        begin, end, allowed, ParseError::kInvalidPromotablePieceType);
  }
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_PARSER_PIECE_TYPE_PARSER_H_
