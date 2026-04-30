cmake_minimum_required(VERSION 3.28.3)

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

find_program(CLANG_FORMAT NAMES clang-format-20 clang-format)
if(CLANG_FORMAT)
  add_custom_target(format
    COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
    COMMENT "Formatting code with ${CLANG_FORMAT}..."
  )
  add_custom_target(format-check
    COMMAND ${CLANG_FORMAT} --dry-run -Werror ${ALL_SOURCE_FILES}
    COMMENT "Checking code format with ${CLANG_FORMAT}..."
  )
endif()

find_program(CLANG_TIDY NAMES clang-tidy-20 clang-tidy)
if(CLANG_TIDY)
  add_custom_target(tidy
    COMMAND ${CLANG_TIDY} ${ALL_SOURCE_FILES} -p ${CMAKE_BINARY_DIR}
      --extra-arg=-Wno-unknown-warning-option
    COMMENT "Linting code with ${CLANG_TIDY}..."
  )
endif()
