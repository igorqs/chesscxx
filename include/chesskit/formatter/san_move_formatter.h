#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_SAN_MOVE_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_SAN_MOVE_FORMATTER_H_

// IWYU pragma: private, include "../san_move.h"

#include <format>
#include <optional>
#include <variant>

#include "../castling_side.h"
#include "../core/san_move.h"
#include "../piece_type.h"
#include "internal/formatter_helper.h"

/// @ingroup SanMoveHelpers
/// @brief formatting support for chesskit::SanCastlingMove
template <>
struct std::formatter<chesskit::SanCastlingMove> : chesskit::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& san, FmtContext& ctx) const {
    return std::format_to(
        ctx.out(), "{}{:[c]}",
        san.side == chesskit::CastlingSide::kKingside ? "O-O" : "O-O-O",
        san.check_indicator);
  }
};

/// @ingroup SanMoveHelpers
/// @brief formatting support for chesskit::SanNormalMove
template <>
struct std::formatter<chesskit::SanNormalMove> : chesskit::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& san, FmtContext& ctx) const {
    std::optional<chesskit::PieceType> piece;
    if (san.piece_type != chesskit::PieceType::kPawn) piece = san.piece_type;

    return std::format_to(ctx.out(), "{:[u]}{}{}{}{:=[u]}{:[c]}", piece,
                          san.origin, san.is_capture ? "x" : "",
                          san.destination, san.promotion, san.check_indicator);
  }
};

/// @ingroup SanMoveHelpers
/// @brief formatting support for chesskit::SanMove
template <>
struct std::formatter<chesskit::SanMove> : chesskit::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& variant, FmtContext& ctx) const {
    return std::visit(
        [&](const auto& arg) { return std::format_to(ctx.out(), "{}", arg); },
        variant);
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_SAN_MOVE_FORMATTER_H_
