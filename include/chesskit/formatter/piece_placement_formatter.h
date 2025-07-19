#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PIECE_PLACEMENT_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PIECE_PLACEMENT_FORMATTER_H_

// IWYU pragma: private, include "../piece_placement.h"

#include <array>
#include <cstddef>
#include <format>
#include <ranges>
#include <string_view>

#include "../core/piece_placement.h"
#include "../file.h"
#include "../rank.h"
#include "internal/formatter_helper.h"

namespace internal {
template <typename RankRange, typename OutputIt>
constexpr auto formatRank(OutputIt out, RankRange&& rank) -> OutputIt {
  int empty_counter = 0;

  for (const auto& piece : rank) {
    if (piece.has_value()) {
      if (empty_counter) {
        out = std::format_to(out, "{}", empty_counter);
        empty_counter = 0;
      }

      out = std::format_to(out, "{:c}", *piece);
    } else {
      ++empty_counter;
    }
  }

  if (empty_counter) out = std::format_to(out, "{}", empty_counter);
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
  auto handleSpec(const chesskit::PiecePlacement& piece_placement, auto& ctx,
                  chesskit::internal::FenSpec /*unused*/) const {
    auto out = ctx.out();

    for (std::size_t rank = 0; rank < chesskit::kNumRanks; ++rank) {
      if (rank != 0) *out++ = '/';
      const auto* begin =
          piece_placement.pieceArray().begin() + ((rank * chesskit::kNumFiles));
      const auto* end = begin + chesskit::kNumFiles;
      out = internal::formatRank(out, std::ranges::subrange(begin, end));
    }

    return out;
  }

  auto handleSpec(const chesskit::PiecePlacement& piece_placement, auto& ctx,
                  chesskit::internal::AsciiSpec /*unused*/) const {
    auto out = ctx.out();

    for (std::size_t rank = 0; rank < chesskit::kNumRanks; ++rank) {
      if (rank != 0) *out++ = '\n';
      const auto* begin =
          piece_placement.pieceArray().begin() + ((rank * chesskit::kNumFiles));
      const auto* end = begin + chesskit::kNumFiles;
      for (const auto& piece : std::ranges::subrange(begin, end)) {
        out = std::format_to(out, "{:[c]?.}", piece);
      }
    }

    return out;
  }

  auto handleSpec(const chesskit::PiecePlacement& piece_placement, auto& ctx,
                  chesskit::internal::PieceListSpec /*unused*/) const {
    auto out = ctx.out();

    bool print_comma = false;

    out = std::format_to(out, "{{ ");
    for (const auto& [color, locationsByType] :
         piece_placement.pieceLocations()) {
      for (const auto& [type, locations] : locationsByType) {
        const std::string_view plural = locations.size() > 1 ? "s" : "";
        out = std::format_to(out, "{}{} {}{}: [", print_comma ? ", " : "",
                             color, type, plural);

        for (auto it = locations.begin(); it != locations.end(); ++it) {
          if (it != locations.begin()) out = std::format_to(out, ", ");
          out = std::format_to(out, "{}", *it);
        }

        out = std::format_to(out, "]");
        print_comma = true;
      }
    }
    out = std::format_to(out, " }}");

    return out;
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_PIECE_PLACEMENT_FORMATTER_H_
