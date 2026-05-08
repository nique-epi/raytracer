raytracer_add_test(
	NAME diffuse_plugin_test
	SOURCES DiffusePluginTest.cpp
	PLUGINS DiffuseMaterial
	LIBS raytracer_core raytracer_math
	INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}
)

raytracer_add_test(
	NAME glossy_plugin_test
	SOURCES GlossyPluginTest.cpp
	PLUGINS GlossyMaterial
	LIBS raytracer_core raytracer_math
	INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}
)
