cmake_minimum_required(VERSION 3.25.0)
project(logg-benchmarks LANGUAGES CXX)

# ------------------------------------------------------------------------------
# dependencies
# ------------------------------------------------------------------------------

find_package(benchmark REQUIRED)

if(PROJECT_IS_TOP_LEVEL)
  find_package(logg REQUIRED)
endif()

include(CTest)
include(GoogleTest)

# ------------------------------------------------------------------------------
# general settings and options
# ------------------------------------------------------------------------------

add_executable(logg-benchmarks)

# ------------------------------------------------------------------------------
# sources
# ------------------------------------------------------------------------------

set(SOURCES logger_bench.cpp main.cpp)

# ------------------------------------------------------------------------------
# target
# ------------------------------------------------------------------------------

target_sources(logg-benchmarks PRIVATE ${SOURCES})

target_link_libraries(logg-benchmarks PRIVATE logg::logg benchmark::benchmark)

target_include_directories(logg-benchmarks PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# ------------------------------------------------------------------------------
# workflow automation
# ------------------------------------------------------------------------------

add_test(NAME logg-benchmarks COMMAND logg-benchmarks --benchmark_color=true)
set_tests_properties(logg-benchmarks PROPERTIES LABELS benchmark)
