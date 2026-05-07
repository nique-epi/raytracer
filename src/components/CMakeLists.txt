add_library(raytracer_components STATIC)

add_subdirectory(image)
add_subdirectory(camera)
add_subdirectory(light)
add_subdirectory(material)
add_subdirectory(Primitives)
add_subdirectory(Transformations)

target_link_libraries(raytracer_components
  PUBLIC
    raytracer_math
    raytracer_core
)

raytracer_enable_target_warnings(raytracer_components)
