add_library(raytracer_factory STATIC)

target_sources(raytracer_factory
  PRIVATE
    PrimitiveFactory.cpp
    LightFactory.cpp
    MaterialFactory.cpp
)

target_include_directories(raytracer_factory
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_factory
  PUBLIC
    raytracer_exceptions
    raytracer_components
    config++
)

raytracer_enable_target_warnings(raytracer_factory)
