add_library(raytracer_material INTERFACE)

target_include_directories(raytracer_material
  INTERFACE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_material
  INTERFACE
    raytracer_math
)

target_sources(raytracer_components
  PRIVATE
    abstract/AMaterial.cpp
    diffuse/DiffuseMaterial.cpp
    glossy/Glossy.cpp
    glass/Glass.cpp
    Textures/solid/SolidColor.cpp
    Textures/checker/CheckerTexture.cpp
)
