add_subdirectory(core)
add_subdirectory(utils/math)
add_subdirectory(plugin)
add_subdirectory(scene)
add_subdirectory(components)
add_subdirectory(output)

add_library(raytracer_src INTERFACE)

target_include_directories(raytracer_src
  INTERFACE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_src
  INTERFACE
    raytracer_core
    raytracer_math
    raytracer_plugin
    raytracer_scene
    raytracer_components
    config++
)

add_library(raytracer_application OBJECT
  Application.cpp
  ArgsParser.cpp
)

target_include_directories(raytracer_application
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_application
  PRIVATE
    raytracer_scene
    raytracer_math
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
