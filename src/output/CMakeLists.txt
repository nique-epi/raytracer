add_library(raytracer_ppm SHARED)

target_sources(raytracer_ppm
  PRIVATE
    ppm/ppm.cpp
)

target_include_directories(raytracer_ppm
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_ppm
  PRIVATE
    raytracer_components
)

set_target_properties(raytracer_ppm PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins"
  PREFIX ""
  SUFFIX ".so"
)

raytracer_enable_target_warnings(raytracer_ppm)
