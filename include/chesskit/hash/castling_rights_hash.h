#ifndef CHESSKIT_INCLUDE_CHESSKIT_HASH_CASTLING_RIGHTS_HASH_H_
#define CHESSKIT_INCLUDE_CHESSKIT_HASH_CASTLING_RIGHTS_HASH_H_

// IWYU pragma: private, include "../castling_rights.h"

#include <bitset>
#include <cstddef>
#include <functional>

#include "../core/castling_rights.h"

/// @ingroup CastlingRightsHelpers
/// @brief hash support for chesskit::CastlingRights
template <>
struct std::hash<chesskit::CastlingRights> {
  size_t operator()(const chesskit::CastlingRights& rights) const {
    using T = std::bitset<chesskit::CastlingRights::kNumCastlingRights>;
    return std::hash<T>{}(rights.toBitset());
  };
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_HASH_CASTLING_RIGHTS_HASH_H_
