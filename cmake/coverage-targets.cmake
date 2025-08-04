set(chesscxx_COVERAGE_COMMAND gcovr CACHE STRING "Coverage command to use")

add_custom_target(
    coverage
    COMMAND "${CMAKE_COMMAND}"
    -D "chesscxx_COVERAGE_COMMAND=${chesscxx_COVERAGE_COMMAND}"
    -D "PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}"
    -D "PROJECT_BINARY_DIR=${PROJECT_BINARY_DIR}"
    -P "${PROJECT_SOURCE_DIR}/cmake/coverage.cmake"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    COMMENT "Building coverage report"
    VERBATIM
)
