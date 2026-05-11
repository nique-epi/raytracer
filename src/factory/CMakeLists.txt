add_library(raytracer_factory STATIC)

target_sources(raytracer_factory
  PRIVATE
    primitive/PrimitiveFactory.cpp
    light/LightFactory.cpp
    material/MaterialFactory.cpp
    camera/CameraFactory.cpp
    transformation/TransformationFactory.cpp
    ComponentFactory.cpp
)

target_include_directories(raytracer_factory
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_factory
  PUBLIC
    raytracer_exceptions
    raytracer_components
    raytracer_translation
    raytracer_rotation
    config++
)

raytracer_enable_target_warnings(raytracer_factory)
