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
)

target_link_libraries(raytracer_tests
  PRIVATE
    raytracer_core
    GTest::gtest_main
)

raytracer_enable_target_warnings(raytracer_tests)

include(GoogleTest)
gtest_discover_tests(raytracer_tests)
