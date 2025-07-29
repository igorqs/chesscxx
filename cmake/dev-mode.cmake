#include(cmake/folders.cmake)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

option(BUILD_DOCS "Build documentation using Doxygen+Breathe+Sphinx" OFF)
if(BUILD_DOCS)
  include(cmake/docs-targets.cmake)
endif()

include(cmake/iwyu-targets.cmake)
include(cmake/lint-targets.cmake)
include(cmake/spell-targets.cmake)

#add_folders(Project)

