raytracer_add_test(
	NAME light_plugin_test
	SOURCES
		LightPluginTest.cpp
		${CMAKE_SOURCE_DIR}/tests/fixtures/SphereFixture.cpp
	PLUGINS raytracer_ambient_light raytracer_directional_light
	LIBS raytracer_core raytracer_math raytracer_scene
	INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/tests
	EXPORT_DYNAMIC
)
