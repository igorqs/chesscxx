#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PIECE_PLACEMENT_MATERIAL_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PIECE_PLACEMENT_MATERIAL_H_

#include <optional>

#include "../../color.h"
#include "../../piece_placement.h"
#include "../../piece_type.h"
#include "square.h"

namespace chesskit::internal {

inline constexpr bool hasOnlyKing(const PiecePlacement& pp,
                                  const Color& color) {
  return pp.pieceLocations().at(color).size() == 1;
}

inline constexpr bool hasOnlyKingAndKnight(const PiecePlacement& pp,
                                           const Color& color) {
  return pp.pieceLocations().at(color).size() == 2 &&
         pp.pieceLocations().at(color).contains(PieceType::kKnight) &&
         pp.pieceLocations().at(color).at(PieceType::kKnight).size() == 1;
}

inline constexpr bool hasOnlyKingAndBishops(const PiecePlacement& pp,
                                            const Color& color) {
  return pp.pieceLocations().at(color).size() == 2 &&
         pp.pieceLocations().at(color).contains(PieceType::kBishop);
}

inline constexpr bool hasBishops(const PiecePlacement& pp) {
  return pp.pieceLocations().at(Color::kWhite).contains(PieceType::kBishop) ||
         pp.pieceLocations().at(Color::kBlack).contains(PieceType::kBishop);
}

inline constexpr bool isKingVsKing(const PiecePlacement& pp) {
  return hasOnlyKing(pp, Color::kWhite) && hasOnlyKing(pp, Color::kBlack);
}

inline constexpr bool isKingAndKnightVsKing(const PiecePlacement& pp) {
  return (hasOnlyKing(pp, Color::kWhite) &&
          hasOnlyKingAndKnight(pp, Color::kBlack)) ||
         (hasOnlyKing(pp, Color::kBlack) &&
          hasOnlyKingAndKnight(pp, Color::kWhite));
}

inline constexpr bool isBishopsOnSameColorDraw(const PiecePlacement& pp) {
  if (!hasBishops(pp)) return false;

  bool validWhite = hasOnlyKing(pp, Color::kWhite) ||
                    hasOnlyKingAndBishops(pp, Color::kWhite);
  bool validBlack = hasOnlyKing(pp, Color::kBlack) ||
                    hasOnlyKingAndBishops(pp, Color::kBlack);

  if (!validWhite || !validBlack) return false;

  std::optional<Color> squareColor;

  for (const auto& color : {Color::kWhite, Color::kBlack}) {
    if (!pp.pieceLocations().at(color).contains(PieceType::kBishop)) continue;

    for (const auto& square :
         pp.pieceLocations().at(color).at(PieceType::kBishop)) {
      if (squareColor.has_value() &&
          squareColor.value() != square_color(square))
        return false;

      squareColor = square_color(square);
    }
  }

  return true;
}

inline constexpr bool isInsufficientMaterialDraw(const PiecePlacement& pp) {
  return isKingVsKing(pp) || isKingAndKnightVsKing(pp) ||
         isBishopsOnSameColorDraw(pp);
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PIECE_PLACEMENT_MATERIAL_H_
