add_library(raytracer_scene STATIC
  CFGSceneLoader.cpp
  SceneLoaderFactory.cpp
  SceneBuilder.cpp
  Scene.cpp
)

target_include_directories(raytracer_scene
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_scene
  PUBLIC
    raytracer_core
    config++
    config
)

raytracer_enable_target_warnings(raytracer_scene)
