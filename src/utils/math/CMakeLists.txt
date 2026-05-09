add_library(raytracer_math STATIC)

target_sources(raytracer_math
  PRIVATE
    Vector3D.cpp
    Ray.cpp
    Color.cpp
    HitRecord.cpp
    AABB.cpp
    RenderSettings.cpp
    Optics.cpp
    Matrix.cpp
)

target_include_directories(raytracer_math
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_math
  PUBLIC
    raytracer_exceptions
)

raytracer_enable_target_warnings(raytracer_math)
