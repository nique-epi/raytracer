include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_subdirectory(helpers)

function(raytracer_add_test)
	set(options EXPORT_DYNAMIC)
	set(oneValueArgs NAME)
	set(multiValueArgs SOURCES PLUGINS LIBS INCLUDES)
	cmake_parse_arguments(ARG
		"${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN}
	)

	if(NOT ARG_NAME)
		message(FATAL_ERROR "raytracer_add_test: NAME is required")
	endif()
	if(NOT ARG_SOURCES)
		message(FATAL_ERROR "raytracer_add_test(${ARG_NAME}): SOURCES is required")
	endif()

	add_executable(${ARG_NAME} ${ARG_SOURCES})

	target_include_directories(${ARG_NAME}
		PRIVATE
			${CMAKE_SOURCE_DIR}/src
			${ARG_INCLUDES}
	)

	target_link_libraries(${ARG_NAME}
		PRIVATE
			raytracer_tests_helpers
			GTest::gtest_main
			${ARG_LIBS}
	)

	if(ARG_EXPORT_DYNAMIC)
		target_link_options(${ARG_NAME}
			PRIVATE
				$<$<PLATFORM_ID:Linux>:-rdynamic>
		)
	endif()

	if(ARG_PLUGINS)
		add_dependencies(${ARG_NAME} ${ARG_PLUGINS})
		foreach(plugin IN LISTS ARG_PLUGINS)
			add_custom_command(TARGET ${ARG_NAME} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different
					$<TARGET_FILE:${plugin}>
					${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:${plugin}>
			)
		endforeach()
	endif()

	raytracer_enable_target_warnings(${ARG_NAME})

	set_target_properties(${ARG_NAME}
		PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	)

	gtest_discover_tests(${ARG_NAME}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	)
endfunction()


add_executable(raytracer_tests
  utils/math/Vector3DTest.cpp
  utils/math/MatrixTest.cpp
  utils/math/RayTest.cpp
  utils/math/ColorTest.cpp
  utils/math/HitRecordTest.cpp
  utils/math/AABBTest.cpp
  utils/math/RenderSettingsTest.cpp
  utils/math/OpticsTest.cpp
  scene/CFGSceneLoaderTest.cpp
  scene/SceneBuilderTest.cpp
  scene/SceneTest.cpp
	scene/background/SolidBackgroundTest.cpp
  exceptions/ExceptionsTest.cpp
  registry/RegistryTest.cpp
  renderer/BasicRendererTest.cpp
  plugin/PluginExceptionTest.cpp
  components/image/ImageTest.cpp
  components/image/ImageExceptionTest.cpp
  components/Primitives/CollectionTest.cpp
  output/ppm/PPMTest.cpp
  ApplicationTest.cpp
  ArgsParserTest.cpp
  factory/ComponentFactoryFixtureTest.cpp
  factory/PrimitiveFactoryTest.cpp
  fixtures/SphereFixture.cpp
  fixtures/OrthoCameraFixture.cpp
  fixtures/ComponentFactoryFixture.cpp
  scene/fixtures/NullLightFixture.cpp
  scene/fixtures/SceneBuilderFixture.cpp
)

target_include_directories(raytracer_tests
	PRIVATE
		${CMAKE_SOURCE_DIR}/src
)

target_compile_definitions(raytracer_tests
  PRIVATE
    SCENES_DIR="${CMAKE_SOURCE_DIR}/scenes"
    SCENE_FIXTURES_DIR="${CMAKE_SOURCE_DIR}/tests/scene/fixtures/cfg"
)

target_link_libraries(raytracer_tests
  PRIVATE
    raytracer_src
    raytracer_tests_helpers
    GTest::gtest_main
)

target_sources(raytracer_tests PRIVATE
  $<TARGET_OBJECTS:raytracer_ppm>
  $<TARGET_OBJECTS:raytracer_application>
)

raytracer_enable_target_warnings(raytracer_tests)

gtest_discover_tests(raytracer_tests)

add_executable(plugin_loader_test
	plugin_loader_test.cpp
)

target_include_directories(plugin_loader_test
	PRIVATE
		${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(plugin_loader_test PRIVATE raytracer_src)

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

add_subdirectory(components/camera)
add_subdirectory(components/light)
add_subdirectory(components/material)
add_subdirectory(components/Primitives)
add_subdirectory(components/Transformations)
