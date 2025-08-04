cmake_minimum_required(VERSION 3.28)

macro(default name)
  if(NOT DEFINED "${name}")
    set("${name}" "${ARGN}")
  endif()
endmacro()

default(chesscxx_COVERAGECOMMAND gcovr)

set(OUTPUT_DIR "${PROJECT_BINARY_DIR}/coverage")
set(OUTPUT_FILE "${OUTPUT_DIR}/index.html")

file(MAKE_DIRECTORY ${OUTPUT_DIR})

execute_process(
  COMMAND "${chesscxx_COVERAGECOMMAND}" -r ${PROJECT_SOURCE_DIR} --html-details ${OUTPUT_FILE}
)
