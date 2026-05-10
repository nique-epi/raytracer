add_library(raytracer_factory INTERFACE)

target_include_directories(raytracer_factory
  INTERFACE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_factory
  INTERFACE
    raytracer_exceptions
    raytracer_components
    config++
)
