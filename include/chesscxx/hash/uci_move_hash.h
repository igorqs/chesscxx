#ifndef CHESSCXX_INCLUDE_CHESSCXX_HASH_UCI_MOVE_HASH_H_
#define CHESSCXX_INCLUDE_CHESSCXX_HASH_UCI_MOVE_HASH_H_

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
/// @brief hash support for chesscxx::UciMove
template <>
struct std::hash<chesscxx::UciMove> {
  auto operator()(const chesscxx::UciMove& uci) const -> size_t {
    constexpr uint8_t kMaxSquareValue = chesscxx::kNumSquares - 1;

    constexpr uint kWidth = std::bit_width(kMaxSquareValue);
    constexpr uint kDoubleWidth = kWidth << 1U;
    using PromotionT = std::optional<chesscxx::PromotablePieceType>;

    return (std::hash<PromotionT>{}(uci.promotion) << kDoubleWidth) |
           (chesscxx::index(uci.origin) << kWidth) |
           chesscxx::index(uci.destination);
  };
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_HASH_UCI_MOVE_HASH_H_
