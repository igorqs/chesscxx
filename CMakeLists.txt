cmake_minimum_required(VERSION 3.28)

include(cmake/prelude.cmake)

project(
  chesscxx
  VERSION 0.1.0
  DESCRIPTION "A C++23 chess library for parsing, formatting, move generation, and validation."
  LANGUAGES CXX
)

include(cmake/variables.cmake)

# ---- Declare library ----

add_library(chesscxx_chesscxx INTERFACE)
add_library(chesscxx::chesscxx ALIAS chesscxx_chesscxx)

set_property(
  TARGET chesscxx_chesscxx PROPERTY
  EXPORT_NAME chesscxx
)

target_include_directories(
  chesscxx_chesscxx ${warning_guard}
  INTERFACE
  "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_compile_features(chesscxx_chesscxx INTERFACE cxx_std_23)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${chesscxx_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(examples)
  endif()
endif()

# ---- Developer mode ----

if(NOT chesscxx_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
    AUTHOR_WARNING
    "Developer mode is intended for developers of chesscxx"
  )
endif()

include(cmake/dev-mode.cmake)

