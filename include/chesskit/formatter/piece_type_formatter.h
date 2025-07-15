#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PIECE_TYPE_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PIECE_TYPE_FORMATTER_H_

// IWYU pragma: private, include "../piece_type.h"

#include <format>
#include <string_view>
#include <utility>

#include "../core/piece_type.h"
#include "internal/formatter_helper.h"

/// @ingroup PieceTypeHelpers
/// @brief formatting support for chesskit::PieceType
template <>
struct std::formatter<chesskit::PieceType>
    : chesskit::internal::SpecDispatcher<chesskit::internal::VerboseSpec,
                                         chesskit::internal::CompactUpperSpec,
                                         chesskit::internal::CompactLowerSpec> {
  auto handleSpec(const auto& value, auto& ctx,
                  chesskit::internal::VerboseSpec) const {
    return std::format_to(ctx.out(), "{}", description(value));
  }

  auto handleSpec(const auto& value, auto& ctx,
                  chesskit::internal::CompactUpperSpec) const {
    return std::format_to(ctx.out(), "{}",
                          static_cast<char>(std::toupper(compact(value))));
  }

  auto handleSpec(const auto& value, auto& ctx,
                  chesskit::internal::CompactLowerSpec) const {
    return std::format_to(ctx.out(), "{}",
                          static_cast<char>(std::tolower(compact(value))));
  }

 private:
  static constexpr auto description(const chesskit::PieceType& type)
      -> std::string_view {
    switch (type) {
      case chesskit::PieceType::kPawn:
        return "pawn";
      case chesskit::PieceType::kKnight:
        return "knight";
      case chesskit::PieceType::kBishop:
        return "bishop";
      case chesskit::PieceType::kRook:
        return "rook";
      case chesskit::PieceType::kQueen:
        return "queen";
      case chesskit::PieceType::kKing:
        return "king";
      default:
        std::unreachable();
    }
  }

  static constexpr auto compact(const chesskit::PieceType& type) -> char {
    switch (type) {
      case chesskit::PieceType::kPawn:
        return 'p';
      case chesskit::PieceType::kKnight:
        return 'n';
      case chesskit::PieceType::kBishop:
        return 'b';
      case chesskit::PieceType::kRook:
        return 'r';
      case chesskit::PieceType::kQueen:
        return 'q';
      case chesskit::PieceType::kKing:
        return 'k';
      default:
        std::unreachable();
    }
  }
};

/**
 *  @ingroup PieceTypeHelpers
 *  @brief formatting support for chesskit::PromotablePieceType
 */
template <>
struct std::formatter<chesskit::PromotablePieceType>
    : std::formatter<chesskit::PieceType> {
  template <typename FmtContext>
  auto format(const auto& type, FmtContext& ctx) const {
    return std::formatter<chesskit::PieceType>::format(
        chesskit::toPieceType(type), ctx);
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PIECE_TYPE_FORMATTER_H_
