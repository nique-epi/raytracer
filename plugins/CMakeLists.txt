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

add_library(DiffuseMaterial SHARED
	../src/components/material/diffuse/DiffuseMaterial.cpp
	../src/components/material/abstract/AMaterial.cpp
)

target_link_libraries(DiffuseMaterial
	PRIVATE
		raytracer_material
)

set_target_properties(DiffuseMaterial
	PROPERTIES
		PREFIX ""
		SUFFIX ".so"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins"
)

raytracer_enable_target_warnings(DiffuseMaterial)

add_library(GlossyMaterial SHARED
	../src/components/material/glossy/Glossy.cpp
	../src/components/material/abstract/AMaterial.cpp
)

target_link_libraries(GlossyMaterial
	PRIVATE
		raytracer_material
)

set_target_properties(GlossyMaterial
	PROPERTIES
		PREFIX ""
		SUFFIX ".so"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins"
)

raytracer_enable_target_warnings(GlossyMaterial)

add_library(raytracer_rotation_transformation SHARED
    ../src/components/Transformations/Rotation/Rotation.cpp
)

target_link_libraries(raytracer_rotation_transformation
    PRIVATE
        raytracer_transformations
)

set_target_properties(raytracer_rotation_transformation PROPERTIES
    PREFIX ""
    SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins"
)

raytracer_enable_target_warnings(raytracer_rotation_transformation)

add_library(raytracer_translation_transformation SHARED
    ../src/components/Transformations/Translation/Translation.cpp
)

target_link_libraries(raytracer_translation_transformation
    PRIVATE
        raytracer_transformations
)

set_target_properties(raytracer_translation_transformation PROPERTIES
    PREFIX ""
    SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins"
)

raytracer_enable_target_warnings(raytracer_translation_transformation)
add_library(Sphere SHARED
	../src/components/Primitives/sphere/Sphere.cpp
)

target_link_libraries(Sphere
	PRIVATE
		raytracer_math
		raytracer_plugin
)

set_target_properties(Sphere
	PROPERTIES
		PREFIX ""
		SUFFIX ".so"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins"
)

raytracer_enable_target_warnings(Sphere)

add_library(Cone SHARED
	../src/components/Primitives/cone/Cone.cpp
)

target_link_libraries(Cone
	PRIVATE
		raytracer_math
		raytracer_plugin
)

set_target_properties(Cone
	PROPERTIES
		PREFIX ""
		SUFFIX ".so"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins"
)

raytracer_enable_target_warnings(Cone)

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
