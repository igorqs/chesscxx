cmake_minimum_required(VERSION 3.28)

project(chesscxxBenchmarks CXX)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(chesscxx REQUIRED)
endif()

find_package(benchmark REQUIRED)

# ---- Benchmarks ----

add_custom_target(run-benchmarks)

#add_custom_target(copy_benchmark_assets
#  COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/data ${PROJECT_BINARY_DIR}/data
#)

function(add_benchmark NAME)
  add_executable("${NAME}" "${NAME}.cpp")
  target_link_libraries("${NAME}" PRIVATE chesscxx::chesscxx benchmark::benchmark)
  target_compile_features("${NAME}" PRIVATE cxx_std_23)

  add_custom_target("run_${NAME}"
    COMMAND "${NAME}"
    VERBATIM
  )

  add_dependencies("run_${NAME}" "${NAME}")
  add_dependencies(run-benchmarks "run_${NAME}")
  #add_dependencies(${NAME} copy_benchmark_assets)
endfunction()

add_benchmark(perft_benchmark)
