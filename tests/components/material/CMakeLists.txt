raytracer_add_test(
	NAME diffuse_plugin_test
	SOURCES DiffusePluginTest.cpp
	PLUGINS DiffuseMaterial
	LIBS raytracer_core raytracer_math
	INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}
)
