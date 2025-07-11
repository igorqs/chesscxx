cmake_minimum_required(VERSION 3.31)

macro(default name)
  if(NOT DEFINED "${name}")
    set("${name}" "${ARGN}")
  endif()
endmacro()

default(IWYU_COMMAND clang-tidy)
default(CACHE_FILE "${CMAKE_BINARY_DIR}/iwyu_processed_files.txt")
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

if(EXISTS "${CACHE_FILE}")
  file(READ "${CACHE_FILE}" cached_data)
  string(REPLACE "\n" ";" processed_files "${cached_data}")
else()
  set(processed_files "")
endif()

foreach(file IN LISTS files)
  list(FIND processed_files "${file}" cached_index)
  if(NOT cached_index EQUAL -1)
    message("[${counter}/${total_files}] skipping already processed \"${file}\"")
  else()
    message("[${counter}/${total_files}] running iwyu command for \"${file}\"")

    execute_process(
        COMMAND "${IWYU_COMMAND}" ${flags} "${file}" -- -xc++ -std=c++23
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        RESULT_VARIABLE result
    )

    if(NOT result EQUAL "0")
      message(FATAL_ERROR "'${file}': iwyu returned with ${result}")
    else()
      file(APPEND "${CACHE_FILE}" "${file}\n")
    endif()
  endif()
  MATH(EXPR counter "${counter}+1")
endforeach()
