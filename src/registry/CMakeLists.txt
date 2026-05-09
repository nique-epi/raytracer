add_library(raytracer_registry STATIC)

target_sources(raytracer_registry
  PRIVATE
    registry.cpp
)

target_include_directories(raytracer_registry
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_registry
  PUBLIC
    raytracer_exceptions
    raytracer_plugin
    raytracer_components
)

raytracer_enable_target_warnings(raytracer_registry)
