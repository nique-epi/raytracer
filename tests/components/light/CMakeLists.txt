raytracer_add_test(
	NAME light_plugin_test
	SOURCES LightPluginTest.cpp
	PLUGINS raytracer_ambient_light raytracer_directional_light
	LIBS raytracer_core raytracer_math
	INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}
)
