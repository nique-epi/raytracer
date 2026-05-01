add_subdirectory(camera)

add_library(raytracer_components STATIC)

add_subdirectory(image)
add_subdirectory(material)
add_subdirectory(Primitives)

target_link_libraries(raytracer_components
  PUBLIC
    raytracer_math
    raytracer_core
)

raytracer_enable_target_warnings(raytracer_components)
