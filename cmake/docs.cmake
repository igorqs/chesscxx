cmake_minimum_required(VERSION 3.28)

macro(default name)
  if(NOT DEFINED "${name}")
    set("${name}" "${ARGN}")
  endif()
endmacro()

default(DOXYGEN_EXECUTABLE doxygen)
default(SPHINX_EXECUTABLE sphinx-build)

# ---- Doxygen ----

set(DOXYGEN_INPUT_DIR "${PROJECT_SOURCE_DIR}/include")
set(DOXYGEN_OUTPUT_DIR "${PROJECT_BINARY_DIR}/docs/doxygen")
set(DOXYFILE_IN "${PROJECT_SOURCE_DIR}/docs/Doxyfile.in")
set(DOXYFILE_OUT "${PROJECT_BINARY_DIR}/docs/Doxyfile")

configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

execute_process(COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT})

# ---- Sphinx ----

set(SPHINX_SOURCE "${PROJECT_SOURCE_DIR}/docs")
set(SPHINX_BUILD "${PROJECT_BINARY_DIR}/docs/sphinx")
set(WORKING_DIR "${PROJECT_BINARY_DIR}/docs")

message(SPHINX_SOURCE="${SPHINX_SOURCE}")
message(SPHINX_BUILD="${SPHINX_BUILD}")
message(WORKING_DIR="${WORKING_DIR}")

execute_process(COMMAND 
                  ${SPHINX_EXECUTABLE} -b html
                  -Dbreathe_projects.chesscxx=${DOXYGEN_OUTPUT_DIR}/xml
                  ${SPHINX_SOURCE} ${SPHINX_BUILD}
                WORKING_DIRECTORY ${WORKING_DIR})
