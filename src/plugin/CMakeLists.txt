add_library(raytracer_plugin STATIC)

target_sources(raytracer_plugin
  PRIVATE
    PluginLoader.cpp
)

target_include_directories(raytracer_plugin
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

find_library(DL_LIBRARY dl)
if(DL_LIBRARY)
  target_link_libraries(raytracer_plugin PUBLIC ${DL_LIBRARY})
endif()

raytracer_enable_target_warnings(raytracer_plugin)
