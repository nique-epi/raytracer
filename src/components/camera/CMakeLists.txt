add_library(raytracer_camera INTERFACE)

target_include_directories(raytracer_camera
	INTERFACE
		${CMAKE_SOURCE_DIR}/src
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(raytracer_camera
	INTERFACE
		raytracer_math
)

target_sources(raytracer_components
	PRIVATE
		perspective/PerspectiveCamera.cpp
)
