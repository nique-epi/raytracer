add_library(raytracer_tests_helpers INTERFACE)

target_include_directories(raytracer_tests_helpers
	INTERFACE
		${CMAKE_SOURCE_DIR}/tests
)

target_link_libraries(raytracer_tests_helpers
	INTERFACE
		${CMAKE_DL_LIBS}
)
