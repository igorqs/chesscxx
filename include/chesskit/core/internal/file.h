#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_FILE_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_FILE_H_

#include <cstdint>
#include <optional>

#include "../../file.h"

namespace chesskit::internal {
inline constexpr uint8_t index(const File& file) {
  return static_cast<uint8_t>(file);
}
inline constexpr std::optional<File> toFile(const uint8_t& index) {
  if (index >= kNumFiles) return std::nullopt;

  return static_cast<File>(index);
}

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_INTERNAL_FILE_H_
