cmake_minimum_required(VERSION 3.31)

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
list(LENGTH files total_files)
set(counter 1)

foreach(file IN LISTS files)
  message("[${counter}/${total_files}] running iwyu command for \"${file}\"")

  execute_process(
      COMMAND "${IWYU_COMMAND}" ${flags} "${file}" -- -xc++ -std=c++23
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
      RESULT_VARIABLE result
  )

  if(NOT result EQUAL "0")
    message(FATAL_ERROR "'${file}': iwyu returned with ${result}")
  endif()
  MATH(EXPR counter "${counter}+1")
endforeach()
