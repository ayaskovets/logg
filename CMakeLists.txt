cmake_minimum_required(VERSION 3.25.0)
project(
  logg
  VERSION 0.0.1
  LANGUAGES CXX)

# ------------------------------------------------------------------------------
# general settings and options
# ------------------------------------------------------------------------------

option(BUILD_BENCHMARKS "Build the benchmarks tree." OFF)

add_library(logg)
add_library(logg::logg ALIAS logg)

# ------------------------------------------------------------------------------
# sources
# ------------------------------------------------------------------------------

include(GenerateExportHeader)

generate_export_header(logg EXPORT_FILE_NAME
                       "${CMAKE_CURRENT_BINARY_DIR}/include/logg/export.hpp")

set(SOURCES src/logger.cpp src/renderer.cpp src/writer.cpp)
set(HEADERS
    "${CMAKE_CURRENT_BINARY_DIR}/include/logg/export.hpp"
    include/logg/level.hpp include/logg/logg.hpp include/logg/logger.hpp
    include/logg/renderer.hpp include/logg/writer.hpp)

# ------------------------------------------------------------------------------
# target
# ------------------------------------------------------------------------------

target_sources(
  logg
  PRIVATE ${SOURCES}
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "${CMAKE_CURRENT_SOURCE_DIR}/include"
         "${CMAKE_CURRENT_BINARY_DIR}/include"
         FILES
         ${HEADERS})
target_compile_features(logg PUBLIC cxx_std_23)
set_target_properties(
  logg
  PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR}
             VERSION ${PROJECT_VERSION}
             POSITION_INDEPENDENT_CODE ON
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN ON)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(logg PUBLIC LOGG_STATIC_DEFINE)
endif()

# ------------------------------------------------------------------------------
# install
# ------------------------------------------------------------------------------

if(PROJECT_IS_TOP_LEVEL)
  include(GNUInstallDirs)
  include(CMakePackageConfigHelpers)

  configure_package_config_file(
    cmake/logg-config.cmake.in logg-config.cmake
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/logg")

  write_basic_package_version_file(logg-config-version.cmake
                                   COMPATIBILITY SameMajorVersion)

  install(
    TARGETS logg
    EXPORT logg_export
    RUNTIME COMPONENT logg
    LIBRARY COMPONENT logg NAMELINK_COMPONENT logg-dev
    ARCHIVE COMPONENT logg-dev
            FILE_SET HEADERS
            COMPONENT logg-dev)

  install(
    EXPORT logg_export
    COMPONENT logg-dev
    FILE "logg-targets.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/logg"
    NAMESPACE logg::)

  install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/logg-config.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/logg-config-version.cmake"
    COMPONENT logg-dev
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/logg")
endif()

# ------------------------------------------------------------------------------
# benchmarks
# ------------------------------------------------------------------------------

if(PROJECT_IS_TOP_LEVEL)
  if(BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
  endif()
endif()

# ------------------------------------------------------------------------------
# tests
# ------------------------------------------------------------------------------
if(PROJECT_IS_TOP_LEVEL)
  include(CTest)

  if(BUILD_TESTING)
    add_subdirectory(tests/unit)
  endif()
endif()
