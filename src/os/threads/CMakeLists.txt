add_library(raytracer_threads STATIC)

target_sources(raytracer_threads
  PRIVATE
    ThreadPool.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/src
    FILES
      ThreadPool.hpp
      ThreadPool.tpp
)

target_include_directories(raytracer_threads
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

find_package(Threads REQUIRED)
target_link_libraries(raytracer_threads
  PUBLIC
    raytracer_helper
    Threads::Threads
)

raytracer_enable_target_warnings(raytracer_threads)
