add_library(raytracer_application OBJECT
  Application.cpp
  ArgsParser.cpp
  ProgressBar.cpp
  SceneAssembler.cpp
)

target_include_directories(raytracer_application
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_application
  PRIVATE
    raytracer_renderer
    raytracer_factory
    raytracer_scene
    raytracer_math
    raytracer_exceptions
)

raytracer_enable_target_warnings(raytracer_application)
