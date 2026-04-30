add_subdirectory(camera)

add_library(raytracer_components STATIC)

target_sources(raytracer_components
  PRIVATE
    Image/Image.cpp
    Image/ImageException.cpp
)

target_include_directories(raytracer_components
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_components
  PUBLIC
    raytracer_math
    raytracer_core
)

raytracer_enable_target_warnings(raytracer_components)
