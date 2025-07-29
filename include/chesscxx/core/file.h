#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_FILE_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_FILE_H_

// IWYU pragma: private, include "../file.h"

#include <cstddef>
#include <cstdint>

namespace chesscxx {
/// @brief Represents the files (columns) on a chessboard.
enum class File : uint8_t {
  kA,
  kB,
  kC,
  kD,
  kE,
  kF,
  kG,
  kH,
};

/// @defgroup FileNonMemberConstants File non-member constants
/// @{

/// @brief The number of files (columns) on a chessboard.
inline constexpr size_t kNumFiles = 8;

// @}

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_FILE_H_
