#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PIECE_PLACEMENT_ERROR_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PIECE_PLACEMENT_ERROR_FORMATTER_H_

// IWYU pragma: private, include "../piece_placement_error.h"

#include <format>
#include <string_view>
#include <utility>

#include "../core/piece_placement_error.h"
#include "internal/formatter_helper.h"

/// @ingroup PiecePlacementErrorHelpers
/// @brief formatting support for chesskit::PiecePlacementError
template <>
struct std::formatter<chesskit::PiecePlacementError>
    : chesskit::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& error, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}", toString(error));
  }

 private:
  [[nodiscard]] constexpr auto toString(const auto& error) const
      -> std::string_view {
    switch (error) {
      case chesskit::PiecePlacementError::kMissingKing:
        return "Missing king";
      case chesskit::PiecePlacementError::kMultipleKingsOfSameColor:
        return "Multiple kings of same color";
      case chesskit::PiecePlacementError::kPawnOnBackRank:
        return "Pawn on back rank";
      case chesskit::PiecePlacementError::kPawnOnPromotionRank:
        return "Pawn on promotion rank";
      default:
        std::unreachable();
    }
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PIECE_PLACEMENT_ERROR_FORMATTER_H_
