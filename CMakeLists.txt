cmake_minimum_required(VERSION 3.10)

project(cbitset
  VERSION 0.3.0
  DESCRIPTION "Simple bitset library in C"
  HOMEPAGE_URL "https://github.com/lemire/cbitset"
  LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

# Detect whether cbitset is built standalone or pulled in via add_subdirectory()/
# FetchContent. Tests and benchmarks are only built for a standalone build.
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CBITSET_IS_TOP_LEVEL ON)
else()
  set(CBITSET_IS_TOP_LEVEL OFF)
endif()

option(CBITSET_BUILD_TESTS "Build the cbitset tests" ${CBITSET_IS_TOP_LEVEL})
option(CBITSET_BUILD_BENCHMARKS "Build the cbitset benchmarks" ${CBITSET_IS_TOP_LEVEL})

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

add_subdirectory(include)
add_subdirectory(src)

if(CBITSET_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(CBITSET_BUILD_BENCHMARKS)
  add_subdirectory(benchmarks)
endif()

# ---------------------------------------------------------------------------
# Installation: make `find_package(cbitset)` work after `cmake --install`.
# ---------------------------------------------------------------------------
include(CMakePackageConfigHelpers)

set(CBITSET_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/cbitset"
  CACHE STRING "Path to cbitset CMake package files")

install(EXPORT cbitset-targets
  NAMESPACE cbitset::
  DESTINATION "${CBITSET_INSTALL_CMAKEDIR}")

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cbitsetConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cbitsetConfig.cmake"
  INSTALL_DESTINATION "${CBITSET_INSTALL_CMAKEDIR}")

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/cbitsetConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/cbitsetConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/cbitsetConfigVersion.cmake"
  DESTINATION "${CBITSET_INSTALL_CMAKEDIR}")
