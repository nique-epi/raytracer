add_library(raytracer_helper INTERFACE)

target_sources(raytracer_helper
  INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/src
    FILES
      Queue.hpp
)

target_include_directories(raytracer_helper
  INTERFACE
    ${CMAKE_SOURCE_DIR}/src
)

find_package(Threads REQUIRED)
target_link_libraries(raytracer_helper
  INTERFACE
    Threads::Threads
)
