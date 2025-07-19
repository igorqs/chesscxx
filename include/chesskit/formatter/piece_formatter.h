#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PIECE_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PIECE_FORMATTER_H_

// IWYU pragma: private, include "../piece.h"

#include <format>

#include "../color.h"
#include "../core/piece.h"
#include "internal/formatter_helper.h"

/// @ingroup PieceHelpers
/// @brief formatting support for chesskit::Piece
template <>
struct std::formatter<chesskit::Piece>
    : chesskit::internal::SpecDispatcher<chesskit::internal::VerboseSpec,
                                         chesskit::internal::CompactSpec> {
  auto handleSpec(const chesskit::Piece& piece, auto& ctx,
                  chesskit::internal::VerboseSpec /*unused*/) const {
    return std::format_to(ctx.out(), "{} {}", piece.color, piece.type);
  }

  auto handleSpec(const chesskit::Piece& piece, auto& ctx,
                  chesskit::internal::CompactSpec /*unused*/) const {
    if (piece.color == chesskit::Color::kBlack) {
      return std::format_to(ctx.out(), "{:l}", piece.type);
    }

    return std::format_to(ctx.out(), "{:u}", piece.type);
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PIECE_FORMATTER_H_
