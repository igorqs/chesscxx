#include(cmake/folders.cmake)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

option(BUILD_BENCHMARKS "Build benchmarks tree." "${chesscxx_DEVELOPER_MODE}")
if(BUILD_BENCHMARKS)
  add_subdirectory(benchmarks)
endif()

include(cmake/coverage-targets.cmake)
include(cmake/docs-targets.cmake)
include(cmake/iwyu-targets.cmake)
include(cmake/lint-targets.cmake)
include(cmake/spell-targets.cmake)

#add_folders(Project)

