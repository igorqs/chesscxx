#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_FILE_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_FILE_H_

// IWYU pragma: private, include "../file.h"

#include <cstdint>

namespace chesskit {
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
inline constexpr uint8_t kNumFiles = 8;

// @}

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_FILE_H_
