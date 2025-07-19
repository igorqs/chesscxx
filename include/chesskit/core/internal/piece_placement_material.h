#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PIECE_PLACEMENT_MATERIAL_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PIECE_PLACEMENT_MATERIAL_H_

#include <optional>

#include "../../color.h"
#include "../../piece_placement.h"
#include "../../piece_type.h"
#include "square.h"

namespace chesskit::internal {

constexpr auto hasOnlyKing(const PiecePlacement& piece_placement,
                           const Color& color) -> bool {
  return piece_placement.pieceLocations().at(color).size() == 1;
}

constexpr auto hasOnlyKingAndKnight(const PiecePlacement& piece_placement,
                                    const Color& color) -> bool {
  return piece_placement.pieceLocations().at(color).size() == 2 &&
         piece_placement.pieceLocations().at(color).contains(
             PieceType::kKnight) &&
         piece_placement.pieceLocations()
                 .at(color)
                 .at(PieceType::kKnight)
                 .size() == 1;
}

constexpr auto hasOnlyKingAndBishops(const PiecePlacement& piece_placement,
                                     const Color& color) -> bool {
  return piece_placement.pieceLocations().at(color).size() == 2 &&
         piece_placement.pieceLocations().at(color).contains(
             PieceType::kBishop);
}

constexpr auto hasBishops(const PiecePlacement& piece_placement) -> bool {
  return piece_placement.pieceLocations()
             .at(Color::kWhite)
             .contains(PieceType::kBishop) ||
         piece_placement.pieceLocations()
             .at(Color::kBlack)
             .contains(PieceType::kBishop);
}

constexpr auto isKingVsKing(const PiecePlacement& piece_placement) -> bool {
  return hasOnlyKing(piece_placement, Color::kWhite) &&
         hasOnlyKing(piece_placement, Color::kBlack);
}

constexpr auto isKingAndKnightVsKing(const PiecePlacement& piece_placement)
    -> bool {
  return (hasOnlyKing(piece_placement, Color::kWhite) &&
          hasOnlyKingAndKnight(piece_placement, Color::kBlack)) ||
         (hasOnlyKing(piece_placement, Color::kBlack) &&
          hasOnlyKingAndKnight(piece_placement, Color::kWhite));
}

constexpr auto isBishopsOnSameColorDraw(const PiecePlacement& piece_placement)
    -> bool {
  if (!hasBishops(piece_placement)) return false;

  bool const valid_white =
      hasOnlyKing(piece_placement, Color::kWhite) ||
      hasOnlyKingAndBishops(piece_placement, Color::kWhite);
  bool const valid_black =
      hasOnlyKing(piece_placement, Color::kBlack) ||
      hasOnlyKingAndBishops(piece_placement, Color::kBlack);

  if (!valid_white || !valid_black) return false;

  std::optional<Color> square_color;

  for (const auto& color : {Color::kWhite, Color::kBlack}) {
    if (!piece_placement.pieceLocations().at(color).contains(
            PieceType::kBishop)) {
      continue;
    }

    for (const auto& square :
         piece_placement.pieceLocations().at(color).at(PieceType::kBishop)) {
      if (square_color.has_value() &&
          square_color.value() != squareColor(square)) {
        return false;
      }

      square_color = squareColor(square);
    }
  }

  return true;
}

constexpr auto isInsufficientMaterialDraw(const PiecePlacement& piece_placement)
    -> bool {
  return isKingVsKing(piece_placement) ||
         isKingAndKnightVsKing(piece_placement) ||
         isBishopsOnSameColorDraw(piece_placement);
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_PIECE_PLACEMENT_MATERIAL_H_
