#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_PIECE_TYPE_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_PIECE_TYPE_FORMATTER_H_

// IWYU pragma: private, include "../piece_type.h"

#include <format>
#include <string_view>
#include <utility>

#include "../core/piece_type.h"
#include "internal/formatter_helper.h"

/// @ingroup PieceTypeHelpers
/// @brief formatting support for chesscxx::PieceType
template <>
struct std::formatter<chesscxx::PieceType>
    : chesscxx::internal::SpecDispatcher<chesscxx::internal::VerboseSpec,
                                         chesscxx::internal::CompactUpperSpec,
                                         chesscxx::internal::CompactLowerSpec> {
  auto handleSpec(const auto& value, auto& ctx,
                  chesscxx::internal::VerboseSpec /*unused*/) const {
    return std::format_to(ctx.out(), "{}", description(value));
  }

  auto handleSpec(const auto& value, auto& ctx,
                  chesscxx::internal::CompactUpperSpec /*unused*/) const {
    return std::format_to(ctx.out(), "{}",
                          static_cast<char>(std::toupper(compact(value))));
  }

  auto handleSpec(const auto& value, auto& ctx,
                  chesscxx::internal::CompactLowerSpec /*unused*/) const {
    return std::format_to(ctx.out(), "{}",
                          static_cast<char>(std::tolower(compact(value))));
  }

 private:
  static constexpr auto description(const chesscxx::PieceType& type)
      -> std::string_view {
    switch (type) {
      case chesscxx::PieceType::kPawn:
        return "pawn";
      case chesscxx::PieceType::kKnight:
        return "knight";
      case chesscxx::PieceType::kBishop:
        return "bishop";
      case chesscxx::PieceType::kRook:
        return "rook";
      case chesscxx::PieceType::kQueen:
        return "queen";
      case chesscxx::PieceType::kKing:
        return "king";
      default:
        std::unreachable();
    }
  }

  static constexpr auto compact(const chesscxx::PieceType& type) -> char {
    switch (type) {
      case chesscxx::PieceType::kPawn:
        return 'p';
      case chesscxx::PieceType::kKnight:
        return 'n';
      case chesscxx::PieceType::kBishop:
        return 'b';
      case chesscxx::PieceType::kRook:
        return 'r';
      case chesscxx::PieceType::kQueen:
        return 'q';
      case chesscxx::PieceType::kKing:
        return 'k';
      default:
        std::unreachable();
    }
  }
};

/**
 *  @ingroup PieceTypeHelpers
 *  @brief formatting support for chesscxx::PromotablePieceType
 */
template <>
struct std::formatter<chesscxx::PromotablePieceType>
    : std::formatter<chesscxx::PieceType> {
  template <typename FmtContext>
  auto format(const auto& type, FmtContext& ctx) const {
    return std::formatter<chesscxx::PieceType>::format(
        chesscxx::toPieceType(type), ctx);
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_PIECE_TYPE_FORMATTER_H_
