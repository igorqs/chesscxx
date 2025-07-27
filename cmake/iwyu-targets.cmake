set(
    IWYU_PATTERNS
    include/*.hpp include/*.h
    CACHE STRING
    "; separated patterns relative to the project source dir to run iwyu"
)

set(IWYU_COMMAND clang-tidy CACHE STRING "IWYU checker to use")

file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/iwyu")

add_custom_target(
    iwyu-check
    COMMAND "${CMAKE_COMMAND}"
    -D "IWYU_COMMAND=${IWYU_COMMAND}"
    -D "PATTERNS=${IWYU_PATTERNS}"
    -P "${PROJECT_SOURCE_DIR}/cmake/iwyu.cmake"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    COMMENT "Checking IWYU"
    VERBATIM
)

add_custom_target(
    iwyu-fix
    COMMAND "${CMAKE_COMMAND}"
    -D "IWYU_COMMAND=${IWYU_COMMAND}"
    -D "PATTERNS=${IWYU_PATTERNS}"
    -D FIX=YES
    -P "${PROJECT_SOURCE_DIR}/cmake/iwyu.cmake"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    COMMENT "Fixing IWYU errors"
    VERBATIM
)
