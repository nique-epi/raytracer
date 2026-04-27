add_executable(raytracer
  main.cpp
)

target_include_directories(raytracer
  PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

raytracer_enable_target_warnings(raytracer)


add_custom_command(
  TARGET raytracer
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:raytracer> ${CMAKE_SOURCE_DIR}/raytracer
)
