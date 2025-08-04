set(DOXYGEN_COMMAND doxygen CACHE STRING "Doxygen command to use")
set(SPHINX_COMMAND sphinx-build CACHE STRING "Sphinx build command to use")

add_custom_target(
    docs
    COMMAND "${CMAKE_COMMAND}"
    -D "DOXYGEN_COMMAND=${DOXYGEN_COMMAND}"
    -D "SPHINX_COMMAND=${SPHINX_COMMAND}"
    -D "PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}"
    -D "PROJECT_BINARY_DIR=${PROJECT_BINARY_DIR}"
    -P "${PROJECT_SOURCE_DIR}/cmake/docs.cmake"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    COMMENT "Building docs"
    VERBATIM
)
