raytracer_add_test(
	NAME camera_plugin_test
	SOURCES PerspectiveCameraPluginTest.cpp
	PLUGINS raytracer_perspective_camera
	LIBS raytracer_registry raytracer_exceptions raytracer_math
)
