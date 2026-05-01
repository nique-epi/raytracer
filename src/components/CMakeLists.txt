add_library(raytracer_components STATIC)

add_subdirectory(image)
add_subdirectory(primitive)
add_subdirectory(camera)
add_subdirectory(light)

target_link_libraries(raytracer_components
  PUBLIC
    raytracer_math
    raytracer_exceptions
)

raytracer_enable_target_warnings(raytracer_components)
