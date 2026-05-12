add_library(raytracer_light INTERFACE)

target_include_directories(raytracer_light
	INTERFACE
		${CMAKE_SOURCE_DIR}/src
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(raytracer_light
	INTERFACE
		raytracer_math
)

target_sources(raytracer_components
	PRIVATE
		ambient/AmbientLight.cpp
		directional/Directional.cpp
)
