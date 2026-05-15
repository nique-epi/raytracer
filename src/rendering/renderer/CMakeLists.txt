add_library(raytracer_renderer STATIC)

target_sources(raytracer_renderer
  PRIVATE
    Tile.cpp
    raytracerRenderer/RaytracerRenderer.cpp
)

target_include_directories(raytracer_renderer
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_renderer
  PUBLIC
    raytracer_components
    raytracer_scene
    raytracer_shading
    raytracer_math
    raytracer_threads
    raytracer_exceptions
)

raytracer_enable_target_warnings(raytracer_renderer)
