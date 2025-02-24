cmake_minimum_required(VERSION 3.9)

project(cbitset VERSION 0.2.2 LANGUAGES C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()
add_subdirectory(include)
add_subdirectory(src)

enable_testing()

add_subdirectory(tests)
add_subdirectory(benchmarks)
