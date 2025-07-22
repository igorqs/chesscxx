#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_CASTLING_RIGHTS_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_CASTLING_RIGHTS_H_

// IWYU pragma: private, include "../castling_rights.h"

#include <bitset>
#include <cstddef>
#include <cstdint>

#include "../castling_side.h"
#include "../color.h"

namespace chesskit {
/// @brief Represents castling rights in a chess game.
class CastlingRights {
 public:
  /// @brief Total number of castling rights.
  static constexpr size_t kNumCastlingRights = 4;
  /// @brief Bit position for white kingside castling right.
  static constexpr size_t kWhiteKingside = 0;
  /// @brief Bit position for white queenside castling right.
  static constexpr size_t kWhiteQueenside = 1;
  /// @brief Bit position for black kingside castling right.
  static constexpr size_t kBlackKingside = 2;
  /// @brief Bit position for black queenside castling right.
  static constexpr size_t kBlackQueenside = 3;

  /// @name Constructors
  /// @{

  /// @brief Default constructor. Initializes all castling rights to true.
  constexpr CastlingRights() = default;

  /// @brief Constructor from a specific bitset value.
  explicit constexpr CastlingRights(std::bitset<kNumCastlingRights> bits)
      : castling_rights_{bits} {}

  /// @}

  /// @name Comparison Operators
  /// @{

  /// @brief Equality comparison operator.
  constexpr auto operator==(const CastlingRights&) const -> bool = default;

  /// @}

  /// @name Element access
  /// @{

  /// @brief Check if castling is allowed for a given side and color.
  [[nodiscard]] auto canCastle(const CastlingSide& side,
                               const Color& color) const -> bool {
    return castling_rights_.test(index(side, color));
  }

  /// @brief Checks if all castling rights are allowed.
  [[nodiscard]] auto all() const -> bool { return castling_rights_.all(); }
  /// @brief Checks if any castling rights are allowed.
  [[nodiscard]] auto any() const -> bool { return castling_rights_.any(); }
  /// @brief Checks if no castling rights are allowed.
  [[nodiscard]] auto none() const -> bool { return castling_rights_.none(); }

  /// @}

  /// @name Modifiers
  /// @{

  /// @brief Enable the castling right for a specific side and color.
  void enable(const CastlingSide& side, const Color& color) {
    castling_rights_.set(index(side, color));
  }
  /// @brief Enable all castling rights for a specific color.
  void enable(const Color& color) {
    castling_rights_.set(index(chesskit::CastlingSide::kKingside, color));
    castling_rights_.set(index(chesskit::CastlingSide::kQueenside, color));
  }
  /// @brief Enable all castling rights.
  void enable() { castling_rights_.set(); }

  /// @brief Disable castling for a specific side and color.
  void disable(const CastlingSide& side, const Color& color) {
    castling_rights_.reset(index(side, color));
  }
  /// @brief Disable all castling rights for a specific color.
  void disable(const Color& color) {
    castling_rights_.reset(index(chesskit::CastlingSide::kKingside, color));
    castling_rights_.reset(index(chesskit::CastlingSide::kQueenside, color));
  }
  /// @brief Disable all castling rights.
  void disable() { castling_rights_.reset(); }

  /// @}

  /// @name Conversions
  /// @{

  /// @brief Returns a bitset representation of the data
  [[nodiscard]] auto toBitset() const
      -> const std::bitset<kNumCastlingRights>& {
    return castling_rights_;
  };

  /// @}

 private:
  static constexpr uint8_t kInitialValue = (1U << kNumCastlingRights) - 1;

  static constexpr auto index(const CastlingSide& side, const Color& color)
      -> uint8_t {
    uint const side_index = (side == CastlingSide::kKingside) ? 0 : 1;
    uint const color_index = (color == Color::kWhite) ? 0 : 1;

    return (color_index << 1U) | side_index;
  }

  std::bitset<kNumCastlingRights> castling_rights_{kInitialValue};
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_CASTLING_RIGHTS_H_
