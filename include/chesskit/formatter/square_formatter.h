#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_SQUARE_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_SQUARE_FORMATTER_H_

// IWYU pragma: private, include "../square.h"

#include <format>

#include "../core/square.h"
#include "internal/formatter_helper.h"

/// @ingroup SquareHelpers
/// @brief formatting support for chesskit::Square
template <>
struct std::formatter<chesskit::Square> : chesskit::internal::NoSpec {
  template <typename FmtContext>
  auto format(const chesskit::Square& square, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}{}", square.file, square.rank);
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_SQUARE_FORMATTER_H_
