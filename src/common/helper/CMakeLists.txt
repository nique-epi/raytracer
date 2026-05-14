add_library(raytracer_helper STATIC)

target_sources(raytracer_helper
  PRIVATE
    Logger.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/src
    FILES
      Logger.hpp
      Queue.hpp
)

target_include_directories(raytracer_helper
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

find_package(Threads REQUIRED)
target_link_libraries(raytracer_helper
  PUBLIC
    Threads::Threads
)

raytracer_enable_target_warnings(raytracer_helper)
