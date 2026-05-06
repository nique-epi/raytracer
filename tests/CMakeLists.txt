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
  scene/SceneTest.cpp
  core/ExceptionsTest.cpp
  core/RegistryTest.cpp
  core/BasicRendererTest.cpp
  plugin/PluginExceptionTest.cpp
  components/image/ImageTest.cpp
  components/image/ImageExceptionTest.cpp
  components/Primitives/CollectionTest.cpp
  output/ppm/PPMTest.cpp
  ApplicationTest.cpp
  ArgsParserTest.cpp
  fixtures/SphereFixture.cpp
  fixtures/OrthoCameraFixture.cpp

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
    GTest::gtest_main
)

target_sources(raytracer_tests PRIVATE
  $<TARGET_OBJECTS:raytracer_ppm>
  $<TARGET_OBJECTS:raytracer_application>
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

add_executable(camera_plugin_test
	components/camera/PerspectiveCameraPluginTest.cpp
)

target_include_directories(camera_plugin_test
	PRIVATE
		${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(camera_plugin_test PRIVATE raytracer_core raytracer_math GTest::gtest_main ${CMAKE_DL_LIBS})


add_dependencies(camera_plugin_test raytracer_perspective_camera)
add_custom_command(TARGET camera_plugin_test POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:raytracer_perspective_camera>
		${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:raytracer_perspective_camera>
)

raytracer_enable_target_warnings(camera_plugin_test)

set_target_properties(camera_plugin_test PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

gtest_discover_tests(camera_plugin_test
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(diffuse_plugin_test
	components/material/DiffusePluginTest.cpp
)

target_include_directories(diffuse_plugin_test
	PRIVATE
		${CMAKE_SOURCE_DIR}/src
		${CMAKE_SOURCE_DIR}/tests/components/material
)

target_link_libraries(diffuse_plugin_test PRIVATE raytracer_core raytracer_math GTest::gtest_main ${CMAKE_DL_LIBS})

add_dependencies(diffuse_plugin_test DiffuseMaterial)

add_custom_command(TARGET diffuse_plugin_test POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:DiffuseMaterial>
		${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:DiffuseMaterial>
)

raytracer_enable_target_warnings(diffuse_plugin_test)

set_target_properties(diffuse_plugin_test PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

gtest_discover_tests(diffuse_plugin_test
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(rotation_transformation_test
	components/Transformations/RotationTransformationTests.cpp
)

target_include_directories(rotation_transformation_test
	PRIVATE
		${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(rotation_transformation_test PRIVATE raytracer_rotation raytracer_math GTest::gtest_main)

raytracer_enable_target_warnings(rotation_transformation_test)

set_target_properties(rotation_transformation_test PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

gtest_discover_tests(rotation_transformation_test
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(translation_transformation_test
	components/Transformations/TranslationTransformationTests.cpp
)

target_include_directories(translation_transformation_test
	PRIVATE
		${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(translation_transformation_test PRIVATE raytracer_translation raytracer_math GTest::gtest_main)

raytracer_enable_target_warnings(translation_transformation_test)

set_target_properties(translation_transformation_test PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

gtest_discover_tests(translation_transformation_test)
add_executable(sphere_plugin_test
	components/Primitives/SpherePluginTest.cpp
)

target_include_directories(sphere_plugin_test
	PRIVATE
		${CMAKE_SOURCE_DIR}/src
		${CMAKE_SOURCE_DIR}/tests/components/Primitives
)

target_link_libraries(sphere_plugin_test PRIVATE raytracer_core raytracer_plugin raytracer_math config++ GTest::gtest_main)
target_link_options(sphere_plugin_test PRIVATE $<$<PLATFORM_ID:Linux>:-rdynamic>)

add_dependencies(sphere_plugin_test Sphere)
add_custom_command(TARGET sphere_plugin_test POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:Sphere>
		${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:Sphere>
)

raytracer_enable_target_warnings(sphere_plugin_test)

set_target_properties(sphere_plugin_test PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

gtest_discover_tests(sphere_plugin_test
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
