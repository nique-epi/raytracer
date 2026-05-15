add_library(raytracer_shading STATIC)

target_sources(raytracer_shading
  PRIVATE
    ShadingContext.cpp
    wireframe/WireframeShader.cpp
    materialPreview/MaterialPreviewShader.cpp
    rendered/RenderedShader.cpp
)

target_include_directories(raytracer_shading
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_shading
  PUBLIC
    raytracer_math
    raytracer_scene
    raytracer_shading_common
    raytracer_integrator
    raytracer_exceptions
)

raytracer_enable_target_warnings(raytracer_shading)
