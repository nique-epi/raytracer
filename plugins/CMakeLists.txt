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

add_library(diffuse SHARED
	../src/components/material/diffuse/DiffuseMaterial.cpp
	../src/components/material/IMaterial.cpp
)

target_link_libraries(diffuse
	PRIVATE
		raytracer_material
)

set_target_properties(diffuse
	PROPERTIES
		PREFIX ""
		SUFFIX ".so"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins"
)

raytracer_enable_target_warnings(diffuse)
