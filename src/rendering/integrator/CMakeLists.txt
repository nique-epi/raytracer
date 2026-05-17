add_library(raytracer_integrator STATIC)

target_sources(raytracer_integrator
  PRIVATE
    whittedIntegrator/WhittedIntegrator.cpp
)

target_include_directories(raytracer_integrator
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_integrator
  PUBLIC
    raytracer_math
    raytracer_scene
    raytracer_rendering_helper
    raytracer_helper
)

raytracer_enable_target_warnings(raytracer_integrator)
