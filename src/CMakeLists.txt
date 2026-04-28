add_library(raytracer_core STATIC)

target_sources(raytracer_core
  PRIVATE
    plugin/PluginLoader.cpp
)

target_include_directories(raytracer_core
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

raytracer_enable_target_warnings(raytracer_core)

find_library(DL_LIBRARY dl)
if(DL_LIBRARY)
  target_link_libraries(raytracer_core PUBLIC ${DL_LIBRARY})
endif()

add_subdirectory(utils/math)

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
