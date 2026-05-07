raytracer_add_test(
	NAME sphere_plugin_test
	SOURCES SpherePluginTest.cpp
	PLUGINS Sphere
	LIBS raytracer_core raytracer_plugin raytracer_math config++
	INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}
	EXPORT_DYNAMIC
)

raytracer_add_test(
	NAME cone_plugin_test
	SOURCES ConePluginTest.cpp
	PLUGINS Cone
	LIBS raytracer_core raytracer_plugin raytracer_math config++
	INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}
	EXPORT_DYNAMIC
)

raytracer_add_test(
	NAME triangle_plugin_test
	SOURCES TrianglePluginTest.cpp
	PLUGINS raytracer_triangle_primitive
	LIBS raytracer_core raytracer_plugin raytracer_math config++
	INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}
	EXPORT_DYNAMIC
)

raytracer_add_test(
	NAME mesh_plugin_test
	SOURCES MeshPluginTest.cpp
	PLUGINS raytracer_mesh_primitive
	LIBS raytracer_core raytracer_plugin raytracer_math config++
	INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}
	EXPORT_DYNAMIC
)
