cmake_minimum_required(VERSION 3.28)

project(chesscxxTests CXX)

#include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(chesscxx REQUIRED)
  enable_testing()
endif()


include(FetchContent)
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.zip
#)
#FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  magic_enum
  URL https://github.com/Neargye/magic_enum/archive/e046b69a3736d314fad813e159b1c192eaef92cd.zip
)
FetchContent_MakeAvailable(magic_enum)

# ---- Tests ----

#include(GoogleTest)

find_package(GTest REQUIRED)
find_package(yaml-cpp REQUIRED)

add_custom_target(copy_test_assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/data ${PROJECT_BINARY_DIR}/data
)

function(add_chesscxx_test NAME)
  add_executable(${NAME} "${NAME}.cpp")
  target_link_libraries(${NAME} PRIVATE yaml-cpp::yaml-cpp magic_enum::magic_enum GTest::gtest_main chesscxx::chesscxx)
  target_compile_features(${NAME} PRIVATE cxx_std_23)
  add_test(NAME ${NAME} COMMAND ${NAME})
  add_dependencies(${NAME} copy_test_assets)
endfunction()

add_chesscxx_test(rank_test)
add_chesscxx_test(file_test)
#add_chesscxx_test(check_indicator_test)
#add_chesscxx_test(castling_side_test)
#add_chesscxx_test(color_test)
#add_chesscxx_test(draw_reason_test)
#add_chesscxx_test(game_result_test)
#add_chesscxx_test(piece_type_test)
#add_chesscxx_test(piece_placement_error_test)
#add_chesscxx_test(position_error_test)
#add_chesscxx_test(move_error_test)
#add_chesscxx_test(parse_error_test)
#add_chesscxx_test(square_test)
#add_chesscxx_test(partial_square_test)
#add_chesscxx_test(piece_test)
#add_chesscxx_test(castling_rights_test)
#add_chesscxx_test(uci_move_test)
#add_chesscxx_test(san_move_test)
#add_chesscxx_test(piece_placement_test)
#add_chesscxx_test(position_test)
#add_chesscxx_test(parse_test)
#add_chesscxx_test(game_test)
#add_chesscxx_test(optional_formatter_test)
#add_chesscxx_test(movegen_test)

# ---- End-of-file commands ----

#add_folders(Examples)

