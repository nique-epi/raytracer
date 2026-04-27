add_executable(plugin_loader_test
	plugin_loader_test.cpp
)

target_include_directories(plugin_loader_test
	PRIVATE
		${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(plugin_loader_test PRIVATE raytracer_core)

raytracer_enable_target_warnings(plugin_loader_test)

add_library(fake_plugin SHARED
	plugins/fake_plugin.cpp
)

set_target_properties(fake_plugin PROPERTIES
	PREFIX ""
	SUFFIX ".so"
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

set_target_properties(plugin_loader_test PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_test(
	NAME plugin_loader_test
	COMMAND plugin_loader_test
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
set_tests_properties(plugin_loader_test PROPERTIES
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
