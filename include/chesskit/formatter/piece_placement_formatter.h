#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PIECE_PLACEMENT_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PIECE_PLACEMENT_FORMATTER_H_

// IWYU pragma: private, include "../piece_placement.h"

#include <array>
#include <format>
#include <ranges>
#include <string_view>

#include "../core/piece_placement.h"
#include "../file.h"
#include "../rank.h"
#include "internal/formatter_helper.h"

namespace internal {
template <typename RankRange, typename OutputIt>
inline constexpr OutputIt formatRank(OutputIt out, RankRange&& rank) {
  int emptyCounter = 0;

  for (const auto& piece : rank) {
    if (piece.has_value()) {
      if (emptyCounter) {
        out = std::format_to(out, "{}", emptyCounter);
        emptyCounter = 0;
      }

      out = std::format_to(out, "{:c}", *piece);
    } else {
      ++emptyCounter;
    }
  }

  if (emptyCounter) out = std::format_to(out, "{}", emptyCounter);
  return out;
}
}  // namespace internal

/// @ingroup PiecePlacementHelpers
/// @brief formatting support for chesskit::PiecePlacement
template <>
struct std::formatter<chesskit::PiecePlacement>
    : chesskit::internal::SpecDispatcher<chesskit::internal::FenSpec,
                                         chesskit::internal::AsciiSpec,
                                         chesskit::internal::PieceListSpec> {
  auto handleSpec(const chesskit::PiecePlacement& pp, auto& ctx,
                  chesskit::internal::FenSpec) const {
    auto out = ctx.out();

    for (int rank = 0; rank < chesskit::kNumRanks; ++rank) {
      if (rank != 0) *out++ = '/';
      auto begin = pp.pieceArray().begin() + rank * chesskit::kNumFiles;
      auto end = begin + chesskit::kNumFiles;
      out = internal::formatRank(out, std::ranges::subrange(begin, end));
    }

    return out;
  }

  auto handleSpec(const chesskit::PiecePlacement& pp, auto& ctx,
                  chesskit::internal::AsciiSpec) const {
    auto out = ctx.out();

    for (int rank = 0; rank < chesskit::kNumRanks; ++rank) {
      if (rank != 0) *out++ = '\n';
      auto begin = pp.pieceArray().begin() + rank * chesskit::kNumFiles;
      auto end = begin + chesskit::kNumFiles;
      for (const auto& piece : std::ranges::subrange(begin, end)) {
        out = std::format_to(out, "{:[c]?.}", piece);
      }
    }

    return out;
  }

  auto handleSpec(const chesskit::PiecePlacement& pp, auto& ctx,
                  chesskit::internal::PieceListSpec) const {
    auto out = ctx.out();

    bool printComma = false;

    out = std::format_to(out, "{{ ");
    for (const auto& [color, locationsByType] : pp.pieceLocations()) {
      for (const auto& [type, locations] : locationsByType) {
        const std::string_view plural = locations.size() > 1 ? "s" : "";
        out = std::format_to(out, "{}{} {}{}: [", printComma ? ", " : "", color,
                             type, plural);

        for (auto it = locations.begin(); it != locations.end(); ++it) {
          if (it != locations.begin()) out = std::format_to(out, ", ");
          out = std::format_to(out, "{}", *it);
        }

        out = std::format_to(out, "]");
        printComma = true;
      }
    }
    out = std::format_to(out, " }}");

    return out;
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PIECE_PLACEMENT_FORMATTER_H_
