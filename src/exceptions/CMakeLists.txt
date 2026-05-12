add_library(raytracer_exceptions STATIC)

target_sources(raytracer_exceptions
  PRIVATE
    Exceptions.cpp
)

target_include_directories(raytracer_exceptions
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

raytracer_enable_target_warnings(raytracer_exceptions)
