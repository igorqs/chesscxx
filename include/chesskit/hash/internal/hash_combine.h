#ifndef CHESSKIT_INCLUDE_CHESSKIT_HASH_INTERNAL_HASH_COMBINE_H_
#define CHESSKIT_INCLUDE_CHESSKIT_HASH_INTERNAL_HASH_COMBINE_H_

#include <cstddef>
#include <functional>
#include <ranges>

namespace chesskit::internal {

template <typename T>
auto merge(const size_t& seed, const T& element) -> size_t {
  constexpr static auto kGoldenRatio = 0x9e3779b9;
  constexpr static auto kLeftShift = 6;
  constexpr static auto kRightShift = 2;

  return seed ^ (std::hash<T>{}(element) + kGoldenRatio + (seed << kLeftShift) +
                 (seed >> kRightShift));
}

template <typename... T>
auto hashCombine(const T&... value) -> size_t {
  size_t seed = 0;

  ((seed = merge(seed, value)), ...);

  return seed;
}

auto hashCombineRange(std::ranges::input_range auto&& range) -> size_t {
  size_t seed = 0;

  for (auto&& element : range) seed = merge(seed, element);

  return seed;
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_HASH_INTERNAL_HASH_COMBINE_H_
