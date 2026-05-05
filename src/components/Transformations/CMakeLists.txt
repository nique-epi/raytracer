add_library(raytracer_transformations INTERFACE)

target_include_directories(raytracer_transformations
  INTERFACE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_transformations
  INTERFACE
    raytracer_math
)

add_library(raytracer_rotation STATIC)

target_sources(raytracer_rotation
  PRIVATE
    Rotation/Rotation.cpp
)

target_link_libraries(raytracer_rotation
  PUBLIC
    raytracer_transformations
)

raytracer_enable_target_warnings(raytracer_rotation)


add_library(raytracer_translation STATIC)

target_sources(raytracer_translation
  PRIVATE
    Translation/Translation.cpp
)

target_link_libraries(raytracer_translation
  PUBLIC
    raytracer_transformations
)

raytracer_enable_target_warnings(raytracer_translation)
