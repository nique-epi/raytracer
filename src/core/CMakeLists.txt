add_library(raytracer_core STATIC)

target_sources(raytracer_core
  PRIVATE
    Exceptions.cpp
    registry/registry.cpp
    renderer/monoThreadRenderer/MonoThreadRenderer.cpp
)

target_include_directories(raytracer_core
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

raytracer_enable_target_warnings(raytracer_core)
