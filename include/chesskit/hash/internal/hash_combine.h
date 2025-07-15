#ifndef CHESSKIT_INCLUDE_CHESSKIT_HASH_INTERNAL_HASH_COMBINE_H_
#define CHESSKIT_INCLUDE_CHESSKIT_HASH_INTERNAL_HASH_COMBINE_H_

#include <cstddef>
#include <functional>
#include <ranges>
#include <type_traits>

namespace chesskit::internal {

template <typename... T>
auto hashCombine(const T&... v) -> size_t {
  size_t seed = 0;

  ((seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2)), ...);

  return seed;
}

auto hashCombineRange(std::ranges::input_range auto&& range) -> size_t {
  size_t seed = 0;

  for (auto&& v : range) {
    using T = std::decay_t<decltype(v)>;
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }

  return seed;
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_HASH_INTERNAL_HASH_COMBINE_H_
