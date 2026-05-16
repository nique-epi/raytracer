cmake_minimum_required(VERSION 3.28.3)

project(raytracer CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_BONUS "Build bonus features" OFF)

# ---------------------------------------------------------------------------
# LOG_LEVEL — compile-time minimum log level.
#
# Translated into RT_LOG_BUILD_LEVEL (0..5) and injected as a compile
# definition. Logger.hpp wraps each logging method in `if constexpr` so
# calls below this level produce no code at all (zero binary footprint).
#
# Defaults: `info` in Release, `trace` elsewhere — debug calls are
# stripped from shipped binaries by default, but kept available during
# development.
# ---------------------------------------------------------------------------
if(NOT DEFINED LOG_LEVEL)
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(LOG_LEVEL "info")
  else()
    set(LOG_LEVEL "trace")
  endif()
endif()

string(TOLOWER "${LOG_LEVEL}" LOG_LEVEL_LOWER)
if(LOG_LEVEL_LOWER STREQUAL "trace")
  set(RT_LOG_BUILD_LEVEL 0)
elseif(LOG_LEVEL_LOWER STREQUAL "debug")
  set(RT_LOG_BUILD_LEVEL 1)
elseif(LOG_LEVEL_LOWER STREQUAL "info")
  set(RT_LOG_BUILD_LEVEL 2)
elseif(LOG_LEVEL_LOWER STREQUAL "warn" OR LOG_LEVEL_LOWER STREQUAL "warning")
  set(RT_LOG_BUILD_LEVEL 3)
elseif(LOG_LEVEL_LOWER STREQUAL "error")
  set(RT_LOG_BUILD_LEVEL 4)
elseif(LOG_LEVEL_LOWER STREQUAL "silent"
       OR LOG_LEVEL_LOWER STREQUAL "off"
       OR LOG_LEVEL_LOWER STREQUAL "none")
  set(RT_LOG_BUILD_LEVEL 5)
else()
  message(FATAL_ERROR
    "Invalid LOG_LEVEL='${LOG_LEVEL}'. "
    "Expected one of: trace, debug, info, warn, error, silent.")
endif()

message(STATUS
  "Log level: ${LOG_LEVEL_LOWER} (RT_LOG_BUILD_LEVEL=${RT_LOG_BUILD_LEVEL})")
add_compile_definitions(RT_LOG_BUILD_LEVEL=${RT_LOG_BUILD_LEVEL})

include(FetchContent)

FetchContent_Declare(
  libconfig
  GIT_REPOSITORY https://github.com/hyperrealm/libconfig.git
  GIT_TAG v1.8.2
)

FetchContent_MakeAvailable(libconfig)


function(raytracer_enable_target_warnings target)
  target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -fPIC)
endfunction()

add_subdirectory(src)

if(BUILD_BONUS)
  add_subdirectory(bonus)
  target_compile_definitions(raytracer PRIVATE BUILD_BONUS)
  target_compile_definitions(raytracer_application PRIVATE BUILD_BONUS)
  target_compile_definitions(raytracer_components PRIVATE BUILD_BONUS)
  target_include_directories(raytracer_components PRIVATE ${CMAKE_SOURCE_DIR}/bonus/stb_image)
  target_sources(raytracer_components PRIVATE
      ${CMAKE_SOURCE_DIR}/bonus/stb_image/stb_image_impl.cpp)
  set_source_files_properties(
      ${CMAKE_SOURCE_DIR}/bonus/stb_image/stb_image_impl.cpp
      PROPERTIES COMPILE_OPTIONS "-w"
  )
  target_include_directories(raytracer_application PRIVATE ${CMAKE_SOURCE_DIR}/bonus)
  target_link_libraries(raytracer_application PRIVATE bonus_assimp_loader)
  target_link_libraries(raytracer PRIVATE bonus_assimp_loader)
  target_link_libraries(raytracer_application PRIVATE bonus_OID_denoiser)
  target_link_libraries(raytracer PRIVATE bonus_OID_denoiser)
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
