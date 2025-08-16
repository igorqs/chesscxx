cmake_minimum_required(VERSION 3.28)

macro(default name)
  if(NOT DEFINED "${name}")
    set("${name}" "${ARGN}")
  endif()
endmacro()

default(IWYU_COMMAND clang-tidy)
default(
    PATTERNS
    include/*.hpp include/*.h
    tests/*.hpp tests/*.h
    examples/*.hpp examples/*.h
)
default(
    COMPILATION_OPTIONS
    -xc++
    -std=c++23
    -Iinclude
)
default(FIX NO)

set(flags
  --checks=-*,misc-include-cleaner
  --warnings-as-errors=*
  -config={\"CheckOptions\":[{\"key\":\"misc-include-cleaner.IgnoreHeaders\",\"value\":\"bits/.*\"}]}
)
if(FIX)
  list(APPEND flags --fix)
endif()

file(GLOB_RECURSE files ${PATTERNS})

execute_process(
  COMMAND ${IWYU_COMMAND} ${flags} ${files} -- ${COMPILATION_OPTIONS}
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
