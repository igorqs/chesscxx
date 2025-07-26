#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_PARTIAL_SQUARE_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_PARTIAL_SQUARE_FORMATTER_H_

// IWYU pragma: private, include "../partial_square.h"

#include <format>

#include "../core/partial_square.h"
#include "internal/formatter_helper.h"

/// @ingroup PartialSquareHelpers
/// @brief formatting support for chesscxx::PartialSquare
template <>
struct std::formatter<chesscxx::PartialSquare> : chesscxx::internal::NoSpec {
  template <typename FmtContext>
  auto format(const chesscxx::PartialSquare& square, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}{}", square.file, square.rank);
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_PARTIAL_SQUARE_FORMATTER_H_
