#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_SAN_MOVE_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_SAN_MOVE_FORMATTER_H_

// IWYU pragma: private, include "../san_move.h"

#include <format>
#include <optional>
#include <variant>

#include "../castling_side.h"
#include "../core/san_move.h"
#include "../piece_type.h"
#include "internal/formatter_helper.h"

/// @ingroup SanMoveHelpers
/// @brief formatting support for chesscxx::SanCastlingMove
template <>
struct std::formatter<chesscxx::SanCastlingMove> : chesscxx::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& san, FmtContext& ctx) const {
    return std::format_to(
        ctx.out(), "{}{:[c]}",
        san.side == chesscxx::CastlingSide::kKingside ? "O-O" : "O-O-O",
        san.check_indicator);
  }
};

/// @ingroup SanMoveHelpers
/// @brief formatting support for chesscxx::SanNormalMove
template <>
struct std::formatter<chesscxx::SanNormalMove> : chesscxx::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& san, FmtContext& ctx) const {
    std::optional<chesscxx::PieceType> piece;
    if (san.piece_type != chesscxx::PieceType::kPawn) piece = san.piece_type;

    return std::format_to(ctx.out(), "{:[u]}{}{}{}{:=[u]}{:[c]}", piece,
                          san.origin, san.is_capture ? "x" : "",
                          san.destination, san.promotion, san.check_indicator);
  }
};

/// @ingroup SanMoveHelpers
/// @brief formatting support for chesscxx::SanMove
template <>
struct std::formatter<chesscxx::SanMove> : chesscxx::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& variant, FmtContext& ctx) const {
    return std::visit(
        [&](const auto& arg) { return std::format_to(ctx.out(), "{}", arg); },
        variant);
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_SAN_MOVE_FORMATTER_H_
