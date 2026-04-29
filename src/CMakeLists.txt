add_subdirectory(core)
add_subdirectory(utils/math)
add_subdirectory(plugin)
add_subdirectory(components)
add_subdirectory(output)

add_library(raytracer_core INTERFACE)

target_include_directories(raytracer_core
  INTERFACE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_core
  INTERFACE
    raytracer_exceptions
    raytracer_math
    raytracer_plugin
    raytracer_components
)

add_executable(raytracer
  main.cpp
)

target_include_directories(raytracer
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer PRIVATE raytracer_core)

raytracer_enable_target_warnings(raytracer)

add_custom_command(
  TARGET raytracer
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:raytracer> ${CMAKE_SOURCE_DIR}/raytracer
)
