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
  COMMAND "${IWYU_COMMAND}" ${flags} ${files} -- -xc++ -std=c++23
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  RESULT_VARIABLE result
)
