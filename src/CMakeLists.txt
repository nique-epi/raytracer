add_library(raytracer_core STATIC)

target_include_directories(raytracer_core
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

raytracer_enable_target_warnings(raytracer_core)

add_subdirectory(Math)

add_executable(raytracer
  main.cpp
)

target_include_directories(raytracer
  PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(raytracer
  PRIVATE
    raytracer_core
)

raytracer_enable_target_warnings(raytracer)

add_custom_command(
  TARGET raytracer
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:raytracer> ${CMAKE_SOURCE_DIR}/raytracer
)
