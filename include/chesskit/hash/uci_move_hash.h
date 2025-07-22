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
  auto operator()(const chesskit::UciMove& uci) const -> size_t {
    constexpr uint8_t kMaxSquareValue = chesskit::kNumSquares - 1;

    constexpr uint kWidth = std::bit_width(kMaxSquareValue);
    constexpr uint kDoubleWidth = kWidth << 1U;
    using PromotionT = std::optional<chesskit::PromotablePieceType>;

    return (std::hash<PromotionT>{}(uci.promotion) << kDoubleWidth) |
           (chesskit::index(uci.origin) << kWidth) |
           chesskit::index(uci.destination);
  };
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_HASH_UCI_MOVE_HASH_H_
