#ifndef CHESSKIT_TESTS_TEST_HELPER_H_
#define CHESSKIT_TESTS_TEST_HELPER_H_

#include <chesskit/formatter/base_formatter.h>
#include <chesskit/hash/internal/hash_combine.h>
#include <chesskit/san_move.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <format>
#include <functional>
#include <magic_enum/magic_enum.hpp>
#include <optional>
#include <ostream>
#include <ranges>
#include <tuple>
#include <variant>
#include <vector>

template <>
struct std::hash<chesskit::SanCastlingMove> {
  size_t operator()(const chesskit::SanCastlingMove& move) const {
    return chesskit::internal::hashCombine(move.side, move.check_indicator);
  };
};

template <>
struct std::hash<chesskit::SanNormalMove> {
  size_t operator()(const chesskit::SanNormalMove& move) const {
    return chesskit::internal::hashCombine(
        move.pieceType, move.origin.file, move.origin.rank, move.isCapture,
        move.destination, move.promotion, move.check_indicator);
  };
};

namespace chesskit {
template <typename T>
  requires std::is_base_of_v<chesskit::internal::BaseFormatter,
                             std::formatter<T>>
inline void PrintTo(const T& obj, std::ostream* os) {
  *os << std::format("{}", obj);
}

inline bool operator==(SanCastlingMove lhs, SanCastlingMove rhs) {
  return lhs.side == rhs.side && lhs.check_indicator == rhs.check_indicator;
}

inline bool operator==(SanNormalMove lhs, SanNormalMove rhs) {
  return lhs.pieceType == rhs.pieceType && lhs.origin.file == rhs.origin.file &&
         lhs.origin.rank == rhs.origin.rank && lhs.isCapture == rhs.isCapture &&
         lhs.destination == rhs.destination && lhs.promotion == rhs.promotion &&
         lhs.check_indicator == rhs.check_indicator;
}

inline bool operator==(SanMove lhs, SanMove rhs) {
  if (lhs.index() != rhs.index()) return false;
  return std::visit([](const auto& a, const auto& b) { return a == b; }, lhs,
                    rhs);
}

namespace testing {
template <typename T, size_t N>
constexpr auto make_optional_array_from_range(const std::array<T, N>& range) {
  std::array<std::optional<T>, N + 1> arr{};
  std::ranges::copy(range, arr.begin());
  return arr;
}

template <typename EnumType>
constexpr auto make_optional_enum_array() {
  constexpr auto values = magic_enum::enum_values<EnumType>();
  return make_optional_array_from_range(values);
}

template <typename Range>
constexpr auto make_distinct_pairs(Range&& range) {
  using T = std::ranges::range_value_t<Range>;
  std::vector<std::tuple<T, T>> pairs;

  const auto begin = std::ranges::begin(range);
  const auto end = std::ranges::end(range);

  for (auto it1 = begin; it1 != end; ++it1)
    for (auto it2 = begin; it2 != end; ++it2)
      if (it1 != it2) pairs.emplace_back(*it1, *it2);

  return pairs;
}
}  // namespace testing
}  // namespace chesskit

#endif  // CHESSKIT_TESTS_TEST_HELPER_H_
