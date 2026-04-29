add_library(raytracer_ppm OBJECT)

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

raytracer_enable_target_warnings(raytracer_ppm)
