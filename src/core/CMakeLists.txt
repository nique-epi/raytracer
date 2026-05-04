add_library(raytracer_core STATIC)

target_sources(raytracer_core
  PRIVATE
    Exceptions.cpp
    registry/registry.cpp
)

target_include_directories(raytracer_core
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

raytracer_enable_target_warnings(raytracer_core)

add_library(raytracer_application OBJECT
  Application.cpp
  ArgsParser.cpp
)

target_include_directories(raytracer_application
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_application
  PRIVATE
    raytracer_core
    raytracer_scene
    raytracer_math
)
