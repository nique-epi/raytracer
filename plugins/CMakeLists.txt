add_library(raytracer_perspective_camera SHARED
	../src/components/camera/perspective/PerspectiveCamera.cpp
)

target_link_libraries(raytracer_perspective_camera
	PRIVATE
		raytracer_camera
)

set_target_properties(raytracer_perspective_camera
	PROPERTIES
		PREFIX ""
		SUFFIX ".so"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins"
)

raytracer_enable_target_warnings(raytracer_perspective_camera)

add_library(raytracer_ambient_light SHARED
	../src/components/light/ambient/AmbientLight.cpp
)

target_link_libraries(raytracer_ambient_light
	PRIVATE
		raytracer_light
)

set_target_properties(raytracer_ambient_light
	PROPERTIES
		PREFIX ""
		OUTPUT_NAME "ambient"
		SUFFIX ".so"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins"
)

raytracer_enable_target_warnings(raytracer_ambient_light)

add_library(raytracer_directional_light SHARED
	../src/components/light/directional/Directional.cpp
)

target_link_libraries(raytracer_directional_light
	PRIVATE
		raytracer_light
)

set_target_properties(raytracer_directional_light
	PROPERTIES
		PREFIX ""
		OUTPUT_NAME "directional"
		SUFFIX ".so"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins"
)

raytracer_enable_target_warnings(raytracer_directional_light)
