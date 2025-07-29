#ifndef CHESSCXX_INCLUDE_CHESSCXX_HASH_CASTLING_RIGHTS_HASH_H_
#define CHESSCXX_INCLUDE_CHESSCXX_HASH_CASTLING_RIGHTS_HASH_H_

// IWYU pragma: private, include "../castling_rights.h"

#include <bitset>
#include <cstddef>
#include <functional>

#include "../core/castling_rights.h"

/// @ingroup CastlingRightsHelpers
/// @brief hash support for chesscxx::CastlingRights
template <>
struct std::hash<chesscxx::CastlingRights> {
  auto operator()(const chesscxx::CastlingRights& rights) const -> size_t {
    using T = std::bitset<chesscxx::CastlingRights::kNumCastlingRights>;
    return std::hash<T>{}(rights.toBitset());
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_HASH_CASTLING_RIGHTS_HASH_H_
