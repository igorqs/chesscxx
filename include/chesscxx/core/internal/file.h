#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_FILE_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_FILE_H_

#include <cstdint>
#include <optional>

#include "../../file.h"

namespace chesscxx::internal {
constexpr auto index(const File& file) -> uint8_t {
  return static_cast<uint8_t>(file);
}
constexpr auto toFile(const uint8_t& index) -> std::optional<File> {
  if (index >= kNumFiles) return std::nullopt;

  return static_cast<File>(index);
}

}  // namespace chesscxx::internal

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_INTERNAL_FILE_H_
