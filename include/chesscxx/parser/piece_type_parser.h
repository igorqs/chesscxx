#ifndef CHESSCXX_INCLUDE_CHESSCXX_PARSER_PIECE_TYPE_PARSER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_PARSER_PIECE_TYPE_PARSER_H_

// IWYU pragma: private, include "../piece_type.h"

#include <expected>
#include <string_view>

#include "../core/piece_type.h"
#include "../parse_error.h"
#include "internal/enum_parser.h"
#include "parse_result.h"
#include "parse_tags.h"
#include "parser.h"

namespace chesscxx {

/// @ingroup PieceTypeHelpers
/// @brief uppercase parsing support for PieceType
template <>
class Parser<PieceType, const char*, parse_as::Uppercase> {
 public:
  static constexpr auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<PieceType, const char*>, ParseError> {
    static constexpr std::string_view kAllowed = "PNBRQK";

    return internal::parseEnum<PieceType>(begin, end, kAllowed,
                                          ParseError::kInvalidPieceType);
  }
};

/// @ingroup PieceTypeHelpers
/// @brief lowercase parsing support for PieceType
template <>
class Parser<PieceType, const char*, parse_as::Lowercase> {
 public:
  static constexpr auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<PieceType, const char*>, ParseError> {
    static constexpr std::string_view kAllowed = "pnbrqk";

    return internal::parseEnum<PieceType>(begin, end, kAllowed,
                                          ParseError::kInvalidPieceType);
  }
};

/// @ingroup PieceTypeHelpers
/// @brief uppercase parsing support for PromotablePieceType
template <>
class Parser<PromotablePieceType, const char*, parse_as::Uppercase> {
 public:
  static constexpr auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<PromotablePieceType, const char*>,
                       ParseError> {
    static constexpr std::string_view kAllowed = "NBRQ";

    return internal::parseEnum<PromotablePieceType>(
        begin, end, kAllowed, ParseError::kInvalidPromotablePieceType);
  }
};

/// @ingroup PieceTypeHelpers
/// @brief lowercase parsing support for PromotablePieceType
template <>
class Parser<PromotablePieceType, const char*, parse_as::Lowercase> {
 public:
  static constexpr auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<PromotablePieceType, const char*>,
                       ParseError> {
    static constexpr std::string_view kAllowed = "nbrq";

    return internal::parseEnum<PromotablePieceType>(
        begin, end, kAllowed, ParseError::kInvalidPromotablePieceType);
  }
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_PARSER_PIECE_TYPE_PARSER_H_
