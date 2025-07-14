#ifndef CHESSKIT_INCLUDE_CHESSKIT_HASH_UCI_MOVE_HASH_H_
#define CHESSKIT_INCLUDE_CHESSKIT_HASH_UCI_MOVE_HASH_H_

// IWYU pragma: private, include "../uci_move.h"

#include <bit>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>

#include "../core/uci_move.h"
#include "../piece_type.h"
#include "../square.h"

/// @ingroup UciMoveHelpers
/// @brief hash support for chesskit::UciMove
template <>
struct std::hash<chesskit::UciMove> {
  size_t operator()(const chesskit::UciMove& uci) const {
    constexpr uint8_t max_square_value = chesskit::kNumSquares - 1;

    constexpr int width = std::bit_width(max_square_value);
    constexpr int double_width = width << 1;
    using PromotionT = std::optional<chesskit::PromotablePieceType>;

    return (std::hash<PromotionT>{}(uci.promotion) << double_width) |
           (chesskit::index(uci.origin) << width) |
           chesskit::index(uci.destination);
  };
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_HASH_UCI_MOVE_HASH_H_
