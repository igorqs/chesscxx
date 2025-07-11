cmake_minimum_required(VERSION 3.31)

include(cmake/prelude.cmake)

project(
  chesskit
  VERSION 1.0.0
  DESCRIPTION "A modern C++23 chess library for parsing, formatting, move generation, and validation."
  LANGUAGES CXX
)

include(cmake/variables.cmake)

# ---- Declare library ----

add_library(chesskit_chesskit INTERFACE)
add_library(chesskit::chesskit ALIAS chesskit_chesskit)

set_property(
  TARGET chesskit_chesskit PROPERTY
  EXPORT_NAME chesskit
)

target_include_directories(
  chesskit_chesskit ${warning_guard}
  INTERFACE
  "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_compile_features(chesskit_chesskit INTERFACE cxx_std_23)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${chesskit_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(examples)
  endif()
endif()

# ---- Developer mode ----

if(NOT chesskit_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
    AUTHOR_WARNING
    "Developer mode is intended for developers of chesskit"
  )
endif()

include(cmake/dev-mode.cmake)

