include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(raytracer_tests
  utils/math/Vector3DTest.cpp
  utils/math/RayTest.cpp
  utils/math/ColorTest.cpp
  utils/math/HitRecordTest.cpp
  utils/math/AABBTest.cpp
  utils/math/RenderSettingsTest.cpp
  utils/math/OpticsTest.cpp
  core/ExceptionsTest.cpp
  plugin/PluginExceptionTest.cpp
  components/image/ImageTest.cpp
  components/image/ImageExceptionTest.cpp
)

target_include_directories(raytracer_tests
	PRIVATE
		${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_tests
  PRIVATE
    raytracer_core
    GTest::gtest_main
)

raytracer_enable_target_warnings(raytracer_tests)

include(GoogleTest)
gtest_discover_tests(raytracer_tests)

add_executable(plugin_loader_test
	plugin_loader_test.cpp
)

target_include_directories(plugin_loader_test
	PRIVATE
		${CMAKE_SOURCE_DIR}/src
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
