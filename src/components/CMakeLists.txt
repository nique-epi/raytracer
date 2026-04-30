add_library(raytracer_components STATIC)

add_subdirectory(image)
add_subdirectory(primitive)

target_link_libraries(raytracer_components
  PUBLIC
    raytracer_math
    raytracer_exceptions
)

raytracer_enable_target_warnings(raytracer_components)
