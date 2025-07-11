#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_PIECE_PLACEMENT_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_PIECE_PLACEMENT_H_

// IWYU pragma: private, include "../piece_placement.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <expected>
#include <optional>
#include <ranges>
#include <unordered_map>
#include <unordered_set>

#include "../color.h"
#include "../piece.h"
#include "../piece_placement_error.h"
#include "../piece_type.h"
#include "../rank.h"
#include "../square.h"
#include "internal/rank.h"
#include "internal/square.h"

namespace chesskit {
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
  constexpr static std::expected<PiecePlacement, PiecePlacementError>
  fromPieceArray(const PieceArray& pieceArray) {
    PiecePlacement piece_placement(pieceArray);

    if (auto error = piece_placement.validationError())
      return std::unexpected(*error);

    return piece_placement;
  }

  /// @}

  /// @name Comparison Operators
  /// @{

  /// @brief Equality comparison operator.
  constexpr bool operator==(const PiecePlacement& other) const {
    return pieceArray_ == other.pieceArray_;
  }

  /// @}

  /// @name Element access
  /// @{

  /// @brief Returns the piece array representing the board.
  const PieceArray& pieceArray() const { return pieceArray_; }

  /// @brief Returns the locations of pieces categorized by type and color.
  const PieceLocationsByTypeAndColor& pieceLocations() const {
    return pieceLocations_;
  }

  /// @}

 private:
  friend class internal::PiecePlacementModifier;

  constexpr PiecePlacement(const PieceArray& pieceArray) {
    for (uint8_t i : std::views::iota(0, static_cast<int8_t>(kNumSquares))) {
      auto sq = internal::createSquarefromIndex(i);
      updatePieceAt(*sq, pieceArray[i]);
    }
  }

  constexpr void updatePieceAt(const Square& square,
                               const std::optional<Piece>& newPiece) {
    if (auto previous = pieceAt(square)) {
      pieceLocations_[previous->color][previous->type].erase(square);
      if (pieceLocations_[previous->color][previous->type].empty())
        pieceLocations_[previous->color].erase(previous->type);
    }

    if (newPiece)
      pieceLocations_[newPiece->color][newPiece->type].insert(square);

    pieceAt(square) = newPiece;
  }

  constexpr std::optional<PiecePlacementError> validationError() const {
    if (isMissingKing(Color::kWhite) || isMissingKing(Color::kBlack))
      return PiecePlacementError::kMissingKing;

    if (hasMultipleKings(Color::kWhite) || hasMultipleKings(Color::kBlack))
      return PiecePlacementError::kMultipleKingsOfSameColor;

    if (hasPawnOnBackRank(Color::kWhite) || hasPawnOnBackRank(Color::kBlack))
      return PiecePlacementError::kPawnOnBackRank;

    if (hasPawnOnPromotionRank(Color::kWhite) ||
        hasPawnOnPromotionRank(Color::kBlack))
      return PiecePlacementError::kPawnOnPromotionRank;

    return std::nullopt;
  }

  constexpr bool isMissingKing(const Color& color) const {
    if (!pieceLocations_.contains(color)) return true;
    if (!pieceLocations_.at(color).contains(PieceType::kKing)) return true;

    return false;
  }

  constexpr bool hasMultipleKings(const Color& color) const {
    return pieceLocations_.at(color).at(PieceType::kKing).size() != 1;
  }

  constexpr bool hasPawnOnBackRank(const Color& color) const {
    return hasPawnOnRank(color, internal::backRank(color));
  }

  constexpr bool hasPawnOnPromotionRank(const Color& color) const {
    return hasPawnOnRank(color, internal::promotionRank(color));
  }

  constexpr bool hasPawnOnRank(const Color& color, const Rank& rank) const {
    if (!pieceLocations_.at(color).contains(PieceType::kPawn)) return false;

    return std::ranges::any_of(
        pieceLocations_.at(color).at(PieceType::kPawn),
        [rank](const Square& square) { return square.rank == rank; });
  }

  constexpr std::optional<Piece>& pieceAt(const Square& square) {
    return pieceArray_[index(square)];
  }

  PieceArray pieceArray_;
  PieceLocationsByTypeAndColor pieceLocations_;
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_PIECE_PLACEMENT_H_
