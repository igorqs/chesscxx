#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_PIECE_TYPE_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_PIECE_TYPE_H_

// IWYU pragma: private, include "../piece_type.h"

#include <cstdint>
#include <utility>

namespace chesskit {

/// @defgroup PieceType PieceType related types
/// @{

/// @brief Represents the type of a chess piece.
enum class PieceType : uint8_t {
  kPawn,
  kKnight,
  kBishop,
  kRook,
  kQueen,
  kKing,
};

/// @brief Represents the types of pieces that a pawn can promote to.
enum class PromotablePieceType : uint8_t {
  kKnight,
  kBishop,
  kRook,
  kQueen,
};

/// @}

/// @defgroup PieceTypeNonMemberFunctions PieceType non-member functions
/// @{

/// @brief Converts a PromotablePieceType to its corresponding PieceType.
inline constexpr auto toPieceType(const PromotablePieceType& promotion)
    -> PieceType {
  switch (promotion) {
    case PromotablePieceType::kKnight:
      return PieceType::kKnight;
    case PromotablePieceType::kBishop:
      return PieceType::kBishop;
    case PromotablePieceType::kRook:
      return PieceType::kRook;
    case PromotablePieceType::kQueen:
      return PieceType::kQueen;
    default:
      std::unreachable();
  }
}

/// @}

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_PIECE_TYPE_H_
