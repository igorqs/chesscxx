cmake_minimum_required(VERSION 3.28)

macro(default name)
  if(NOT DEFINED "${name}")
    set("${name}" "${ARGN}")
  endif()
endmacro()

default(DOXYGEN_COMMAND doxygen)
default(SPHINX_COMMAND sphinx-build)
default(EXAMPLES_SOURCE_DIR "${PROJECT_SOURCE_DIR}")
default(EXAMPLES_BINARY_DIR "${PROJECT_BINARY_DIR}")

# ---- Doxygen ----

set(DOXYGEN_INPUT_DIR "${PROJECT_SOURCE_DIR}/include")
set(DOXYGEN_OUTPUT_DIR "${PROJECT_BINARY_DIR}/docs/doxygen")
set(DOXYFILE_IN "${PROJECT_SOURCE_DIR}/docs/Doxyfile.in")
set(DOXYFILE_OUT "${PROJECT_BINARY_DIR}/docs/Doxyfile")

configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

execute_process(COMMAND ${DOXYGEN_COMMAND} ${DOXYFILE_OUT})

# ---- Sphinx ----

set(SPHINX_SOURCE "${PROJECT_SOURCE_DIR}/docs")
set(SPHINX_BUILD "${PROJECT_BINARY_DIR}/docs/sphinx")
file(RELATIVE_PATH EXAMPLES_RELATIVE_SOURCE_DIR "${SPHINX_SOURCE}" "${EXAMPLES_SOURCE_DIR}/examples")
file(RELATIVE_PATH EXAMPLES_RELATIVE_BINARY_DIR "${SPHINX_SOURCE}" "${EXAMPLES_BINARY_DIR}/examples")

execute_process(COMMAND 
                  ${CMAKE_COMMAND} -E env 
                    "EXAMPLES_RELATIVE_SOURCE_DIR=${EXAMPLES_RELATIVE_SOURCE_DIR}"
                    "EXAMPLES_RELATIVE_BINARY_DIR=${EXAMPLES_RELATIVE_BINARY_DIR}"
                  ${SPHINX_COMMAND} -b html
                    -Dbreathe_projects.chesscxx=${DOXYGEN_OUTPUT_DIR}/xml
                    ${SPHINX_SOURCE} ${SPHINX_BUILD}
                )
