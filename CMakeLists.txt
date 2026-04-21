cmake_minimum_required(VERSION 3.28)

project(raytracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

add_subdirectory(src)

option(WITH_TESTS "Build unit tests" OFF)

if (WITH_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
