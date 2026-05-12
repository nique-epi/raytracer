add_library(raytracer_config STATIC)

target_sources(raytracer_config
  PRIVATE
    ConfigException.cpp
)

target_include_directories(raytracer_config
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_config
  PUBLIC
    raytracer_exceptions
)

raytracer_enable_target_warnings(raytracer_config)
