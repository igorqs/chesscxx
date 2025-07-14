cmake_minimum_required(VERSION 3.31)

project(chesskitExamples CXX)

#include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(chesskit REQUIRED)
endif()

# ---- Examples ----

add_custom_target(run-examples)

add_custom_target(copy_example_assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/data ${PROJECT_BINARY_DIR}/data
)

function(add_example NAME)
  add_executable("${NAME}" "${NAME}.cpp")
  target_link_libraries("${NAME}" PRIVATE chesskit::chesskit)
  target_compile_features("${NAME}" PRIVATE cxx_std_23)

  add_custom_target("run_${NAME}"
    COMMAND "${NAME}" | tee "${NAME}.out"
    VERBATIM
  )

  add_dependencies("run_${NAME}" "${NAME}")
  add_dependencies(run-examples "run_${NAME}")
  add_dependencies(${NAME} copy_example_assets)
endfunction()

add_example(rank_usage)
add_example(file_usage)
add_example(piece_type_usage)
add_example(castling_side_usage)
add_example(check_indicator_usage)
add_example(color_usage)
add_example(draw_reason_usage)
add_example(game_result_usage)
add_example(move_error_usage)
add_example(piece_placement_error_usage)
add_example(position_error_usage)
add_example(square_usage)
add_example(partial_square_usage)
add_example(piece_usage)
add_example(castling_rights_usage)
add_example(castling_rights_parse_usage)
add_example(uci_move_usage)
add_example(san_castling_move_usage)
add_example(san_normal_move_usage)
add_example(san_move_usage)
add_example(piece_placement_usage)
add_example(position_usage)
add_example(parse_error_usage)
add_example(parse_from_usage)
add_example(parse_result_usage)
add_example(parse_as_usage)
add_example(parser_usage)
add_example(optional_formatter_usage)
add_example(position_hash_usage)
add_example(position_legal_en_passant_usage)
add_example(game_usage)
add_example(game_general_usage)
add_example(game_repetition_usage)
add_example(game_hash_usage)
add_example(movegen_usage)
add_example(movegen_castling_usage)
add_example(movegen_promotion_usage)
add_example(basic_full_game_usage)
add_example(basic_pgn_usage)

# ---- End-of-file commands ----

#add_folders(Examples)

