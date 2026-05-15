add_library(raytracer_shading_common STATIC)

target_sources(raytracer_shading_common
  PRIVATE
    ShadingHelpers.cpp
)

target_include_directories(raytracer_shading_common
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_shading_common
  PUBLIC
    raytracer_math
)

raytracer_enable_target_warnings(raytracer_shading_common)
