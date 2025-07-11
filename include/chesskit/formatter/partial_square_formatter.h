#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PARTIAL_SQUARE_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PARTIAL_SQUARE_FORMATTER_H_

// IWYU pragma: private, include "../partial_square.h"

#include <format>

#include "../core/partial_square.h"
#include "internal/formatter_helper.h"

/// @ingroup PartialSquareHelpers
/// @brief formatting support for chesskit::PartialSquare
template <>
struct std::formatter<chesskit::PartialSquare> : chesskit::internal::NoSpec {
  template <typename FmtContext>
  auto format(const chesskit::PartialSquare& square, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}{}", square.file, square.rank);
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PARTIAL_SQUARE_FORMATTER_H_
