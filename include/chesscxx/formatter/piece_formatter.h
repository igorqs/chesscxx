#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_PIECE_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_PIECE_FORMATTER_H_

// IWYU pragma: private, include "../piece.h"

#include <format>

#include "../color.h"
#include "../core/piece.h"
#include "internal/formatter_helper.h"

/// @ingroup PieceHelpers
/// @brief formatting support for chesscxx::Piece
template <>
struct std::formatter<chesscxx::Piece>
    : chesscxx::internal::SpecDispatcher<chesscxx::internal::VerboseSpec,
                                         chesscxx::internal::CompactSpec> {
  auto handleSpec(const chesscxx::Piece& piece, auto& ctx,
                  chesscxx::internal::VerboseSpec /*unused*/) const {
    return std::format_to(ctx.out(), "{} {}", piece.color, piece.type);
  }

  auto handleSpec(const chesscxx::Piece& piece, auto& ctx,
                  chesscxx::internal::CompactSpec /*unused*/) const {
    if (piece.color == chesscxx::Color::kBlack) {
      return std::format_to(ctx.out(), "{:l}", piece.type);
    }

    return std::format_to(ctx.out(), "{:u}", piece.type);
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_PIECE_FORMATTER_H_
