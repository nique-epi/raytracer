add_subdirectory(exceptions)
add_subdirectory(config)
add_subdirectory(utils/math)
add_subdirectory(common)
add_subdirectory(os)
add_subdirectory(components)
add_subdirectory(factory)
add_subdirectory(scene)
add_subdirectory(shading_common)
add_subdirectory(integrator)
add_subdirectory(shading)
add_subdirectory(renderer)
add_subdirectory(application)
add_subdirectory(output)

add_library(raytracer_src INTERFACE)

target_include_directories(raytracer_src
  INTERFACE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_src
  INTERFACE
    raytracer_exceptions
    raytracer_config
    raytracer_math
    raytracer_helper
    raytracer_threads
    raytracer_components
    raytracer_factory
    raytracer_scene
    raytracer_shading_common
    raytracer_integrator
    raytracer_shading
    raytracer_renderer
    config++
)

add_executable(raytracer
  main.cpp
  $<TARGET_OBJECTS:raytracer_application>
  $<TARGET_OBJECTS:raytracer_ppm>
)

target_include_directories(raytracer
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer PRIVATE raytracer_src)

raytracer_enable_target_warnings(raytracer)

add_custom_command(
  TARGET raytracer
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:raytracer> ${CMAKE_SOURCE_DIR}/raytracer
)
