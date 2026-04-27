cmake_minimum_required(VERSION 3.16)

project(raytracer CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_BONUS "Build bonus features" OFF)

find_package(libconfig++ QUIET)

if(BUILD_BONUS)
  find_package(SFML QUIET COMPONENTS graphics window system)
endif()

function(raytracer_enable_target_warnings target)
  target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -fPIC)
endfunction()

add_subdirectory(src)
add_subdirectory(plugins)

if(BUILD_BONUS)
  add_subdirectory(bonus)
endif()

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
