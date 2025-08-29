cmake_minimum_required(VERSION 3.28)

project(chesscxxBenchmarks CXX)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(chesscxx REQUIRED)
endif()

find_package(benchmark REQUIRED)

# ---- Benchmarks ----

add_custom_target(run-benchmarks)

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
endfunction()

add_benchmark(perft_benchmark)
