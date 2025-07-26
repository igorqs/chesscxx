#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_SQUARE_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_SQUARE_FORMATTER_H_

// IWYU pragma: private, include "../square.h"

#include <format>

#include "../core/square.h"
#include "internal/formatter_helper.h"

/// @ingroup SquareHelpers
/// @brief formatting support for chesscxx::Square
template <>
struct std::formatter<chesscxx::Square> : chesscxx::internal::NoSpec {
  template <typename FmtContext>
  auto format(const chesscxx::Square& square, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}{}", square.file, square.rank);
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_SQUARE_FORMATTER_H_
