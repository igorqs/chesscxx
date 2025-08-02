#include(cmake/folders.cmake)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

include(cmake/docs-targets.cmake)
include(cmake/iwyu-targets.cmake)
include(cmake/lint-targets.cmake)
include(cmake/spell-targets.cmake)

#add_folders(Project)

