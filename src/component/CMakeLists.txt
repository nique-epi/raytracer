add_library(raytracer_component INTERFACE)

target_include_directories(raytracer_component
	INTERFACE
		${CMAKE_SOURCE_DIR}/src
)

add_subdirectory(camera)

target_link_libraries(raytracer_component
	INTERFACE
		raytracer_camera
)
