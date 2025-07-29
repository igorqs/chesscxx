#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_PIECE_PLACEMENT_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_PIECE_PLACEMENT_H_

// IWYU pragma: private, include "../piece_placement.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <expected>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "../color.h"
#include "../piece.h"
#include "../piece_placement_error.h"
#include "../piece_type.h"
#include "../rank.h"
#include "../square.h"
#include "internal/rank.h"
#include "internal/square.h"

namespace chesscxx {
namespace internal {
// Forward declaration
class PiecePlacementModifier;
}  // namespace internal

/// @brief Represents the piece placement on a chess board.
class PiecePlacement {
 public:
  /// @brief Type alias for an array representing all squares on the chessboard,
  /// where each square may contain a piece or be empty (represented by
  /// std::nullopt).
  using PieceArray = std::array<std::optional<Piece>, kNumSquares>;
  /// @brief Type alias for a set of squares occupied by pieces.
  using PieceLocations = std::unordered_set<Square>;
  /// @brief Type alias for mapping from PieceType to PieceLocations.
  using PieceLocationsByType = std::unordered_map<PieceType, PieceLocations>;
  /// @brief Type alias for mapping from Color to PieceLocationsByType.
  using PieceLocationsByTypeAndColor =
      std::unordered_map<Color, PieceLocationsByType>;

  /// @name Constructors
  /// @{

  /// @brief Default constructor. Initializes the board with the standard chess
  /// starting position.
  constexpr PiecePlacement();

  /// @}

  /// @name Static creation methods
  /// @{

  /// @brief Creates a PiecePlacement object from a PieceArray, or returns an
  /// error if validation fails.
  constexpr static auto fromPieceArray(const PieceArray& piece_array)
      -> std::expected<PiecePlacement, PiecePlacementError> {
    PiecePlacement piece_placement(piece_array);

    if (auto error = piece_placement.validationError()) {
      return std::unexpected(*error);
    }

    return piece_placement;
  }

  /// @}

  /// @name Comparison Operators
  /// @{

  /// @brief Equality comparison operator.
  constexpr auto operator==(const PiecePlacement& other) const -> bool {
    return piece_array_ == other.piece_array_;
  }

  /// @}

  /// @name Element access
  /// @{

  /// @brief Returns the piece array representing the board.
  auto pieceArray() const -> const PieceArray& { return piece_array_; }

  /// @brief Returns the locations of pieces categorized by type and color.
  auto pieceLocations() const -> const PieceLocationsByTypeAndColor& {
    return piece_locations_;
  }

  /// @}

 private:
  friend class internal::PiecePlacementModifier;

  constexpr explicit PiecePlacement(const PieceArray& piece_array) {
    for (uint8_t i = 0; i < kNumSquares; ++i) {
      auto square = internal::createSquareFromIndex(i);
      if (square.has_value()) {
        updatePieceAt(*square, piece_array.at(i));
      } else {
        std::unreachable();
      }
    }
  }

  constexpr void updatePieceAt(const Square& square,
                               const std::optional<Piece>& new_piece) {
    if (auto previous = pieceAt(square)) {
      piece_locations_[previous->color][previous->type].erase(square);
      if (piece_locations_[previous->color][previous->type].empty()) {
        piece_locations_[previous->color].erase(previous->type);
      }
    }

    if (new_piece) {
      piece_locations_[new_piece->color][new_piece->type].insert(square);
    }

    pieceAt(square) = new_piece;
  }

  constexpr auto validationError() const -> std::optional<PiecePlacementError> {
    if (isMissingKing(Color::kWhite) || isMissingKing(Color::kBlack)) {
      return PiecePlacementError::kMissingKing;
    }

    if (hasMultipleKings(Color::kWhite) || hasMultipleKings(Color::kBlack)) {
      return PiecePlacementError::kMultipleKingsOfSameColor;
    }

    if (hasPawnOnBackRank(Color::kWhite) || hasPawnOnBackRank(Color::kBlack)) {
      return PiecePlacementError::kPawnOnBackRank;
    }

    if (hasPawnOnPromotionRank(Color::kWhite) ||
        hasPawnOnPromotionRank(Color::kBlack)) {
      return PiecePlacementError::kPawnOnPromotionRank;
    }

    return std::nullopt;
  }

  constexpr auto isMissingKing(const Color& color) const -> bool {
    if (!piece_locations_.contains(color)) return true;
    if (!piece_locations_.at(color).contains(PieceType::kKing)) return true;

    return false;
  }

  constexpr auto hasMultipleKings(const Color& color) const -> bool {
    return piece_locations_.at(color).at(PieceType::kKing).size() != 1;
  }

  constexpr auto hasPawnOnBackRank(const Color& color) const -> bool {
    return hasPawnOnRank(color, internal::backRank(color));
  }

  constexpr auto hasPawnOnPromotionRank(const Color& color) const -> bool {
    return hasPawnOnRank(color, internal::promotionRank(color));
  }

  constexpr auto hasPawnOnRank(const Color& color, const Rank& rank) const
      -> bool {
    if (!piece_locations_.at(color).contains(PieceType::kPawn)) return false;

    return std::ranges::any_of(
        piece_locations_.at(color).at(PieceType::kPawn),
        [rank](const Square& square) { return square.rank == rank; });
  }

  constexpr auto pieceAt(const Square& square) -> std::optional<Piece>& {
    return piece_array_.at(index(square));
  }

  PieceArray piece_array_;
  PieceLocationsByTypeAndColor piece_locations_;
};

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_PIECE_PLACEMENT_H_
