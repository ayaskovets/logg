cmake_minimum_required(VERSION 3.25.0)
project(logg-unit-tests LANGUAGES CXX)

# ------------------------------------------------------------------------------
# dependencies
# ------------------------------------------------------------------------------

find_package(GTest REQUIRED)

if(PROJECT_IS_TOP_LEVEL)
  find_package(logg REQUIRED)
endif()

include(CTest)
include(GoogleTest)

# ------------------------------------------------------------------------------
# general settings and options
# ------------------------------------------------------------------------------

add_executable(logg-unit-tests)

# ------------------------------------------------------------------------------
# sources
# ------------------------------------------------------------------------------

set(SOURCES logger_test.cpp renderer_test.cpp writer_test.cpp)

# ------------------------------------------------------------------------------
# target
# ------------------------------------------------------------------------------

target_sources(logg-unit-tests PRIVATE ${SOURCES})

target_link_libraries(logg-unit-tests PRIVATE logg::logg GTest::gtest_main)

gtest_discover_tests(logg-unit-tests)
