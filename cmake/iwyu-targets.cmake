set(
    IWYU_PATTERNS
    include/*.hpp include/*.h
    tests/*.hpp tests/*.h
    examples/*.hpp examples/*.h
    CACHE STRING
    "; separated patterns relative to the project source dir to run iwyu"
)

set(
    COMPILATION_OPTIONS
    -xc++
    -std=c++23
    -Iinclude
    CACHE STRING
    "; separated compilation options for the iwyu command"
)

set(IWYU_COMMAND clang-tidy CACHE STRING "IWYU checker to use")

add_custom_target(
    iwyu-check
    COMMAND "${CMAKE_COMMAND}"
    -D "IWYU_COMMAND=${IWYU_COMMAND}"
    -D "PATTERNS=${IWYU_PATTERNS}"
    -D "COMPILATION_OPTIONS=${COMPILATION_OPTIONS}"
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
    -D "COMPILATION_OPTIONS=${COMPILATION_OPTIONS}"
    -D FIX=YES
    -P "${PROJECT_SOURCE_DIR}/cmake/iwyu.cmake"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    COMMENT "Fixing IWYU errors"
    VERBATIM
)
