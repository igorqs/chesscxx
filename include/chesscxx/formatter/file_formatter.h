#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_FILE_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_FILE_FORMATTER_H_

// IWYU pragma: private, include "../file.h"

#include <cstdint>
#include <format>

#include "../core/file.h"
#include "internal/formatter_helper.h"

/// @ingroup FileHelpers
/// @brief formatting support for chesscxx::File
template <>
struct std::formatter<chesscxx::File> : chesscxx::internal::NoSpec {
  template <typename FmtContext>
  auto format(const auto& file, FmtContext& ctx) const {
    return std::format_to(ctx.out(), "{}", compact(file));
  }

 private:
  [[nodiscard]] constexpr auto compact(const auto& file) const -> char {
    return static_cast<char>('a' + static_cast<uint8_t>(file));
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_FILE_FORMATTER_H_
