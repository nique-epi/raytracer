add_library(raytracer_rendering_helper STATIC)

target_sources(raytracer_rendering_helper
  PRIVATE
    RenderingHelpers.cpp
)

target_include_directories(raytracer_rendering_helper
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_rendering_helper
  PUBLIC
    raytracer_math
)

raytracer_enable_target_warnings(raytracer_rendering_helper)
