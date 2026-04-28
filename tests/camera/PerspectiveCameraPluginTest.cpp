#include <dlfcn.h>
#include <gtest/gtest.h>
#include <filesystem>
#include "component/camera/ICamera.hpp"

namespace {
using CreateCameraFn = ICamera* (*)();
using DestroyCameraFn = void (*)(ICamera*);

std::filesystem::path findPluginPath() {
  const std::filesystem::path candidates[] = {
      std::filesystem::current_path() / "raytracer_perspective_camera.so",
      std::filesystem::current_path() /
          "../plugins/raytracer_perspective_camera.so",
      std::filesystem::current_path() /
          "../../plugins/raytracer_perspective_camera.so",
      std::filesystem::current_path() /
          "../../../plugins/raytracer_perspective_camera.so",
  };

  for (const auto& candidate : candidates) {
    if (std::filesystem::exists(candidate)) {
      return candidate;
    }
  }

  return {};
}
}  // namespace

TEST(PerspectiveCameraPluginTest, ExposesCreateAndDestroyEntryPoints) {
  const std::filesystem::path pluginPath = findPluginPath();
  ASSERT_FALSE(pluginPath.empty())
      << "Could not find raytracer_perspective_camera.so";

  void* handle = dlopen(pluginPath.c_str(), RTLD_NOW | RTLD_LOCAL);
  ASSERT_NE(handle, nullptr) << dlerror();

  dlerror();
  void* createSymbol = dlsym(handle, "createCamera");
  ASSERT_EQ(dlerror(), nullptr) << "Failed to resolve createCamera";
  ASSERT_NE(createSymbol, nullptr);

  dlerror();
  void* destroySymbol = dlsym(handle, "DestroyCamera");
  ASSERT_EQ(dlerror(), nullptr) << "Failed to resolve DestroyCamera";
  ASSERT_NE(destroySymbol, nullptr);

  auto createCamera = reinterpret_cast<CreateCameraFn>(createSymbol);
  auto destroyCamera = reinterpret_cast<DestroyCameraFn>(destroySymbol);

  ICamera* camera = createCamera();
  ASSERT_NE(camera, nullptr);

  destroyCamera(camera);
  ASSERT_EQ(dlclose(handle), 0);
}

TEST(PerspectiveCameraPluginTest, CenterRayPointsForward) {
  const std::filesystem::path pluginPath = findPluginPath();
  ASSERT_FALSE(pluginPath.empty())
      << "Could not find raytracer_perspective_camera.so";

  void* handle = dlopen(pluginPath.c_str(), RTLD_NOW | RTLD_LOCAL);
  ASSERT_NE(handle, nullptr) << dlerror();

  dlerror();
  auto* createSymbol = dlsym(handle, "createCamera");
  ASSERT_EQ(dlerror(), nullptr) << "Failed to resolve createCamera";
  ASSERT_NE(createSymbol, nullptr);

  dlerror();
  auto* destroySymbol = dlsym(handle, "DestroyCamera");
  ASSERT_EQ(dlerror(), nullptr) << "Failed to resolve DestroyCamera";
  ASSERT_NE(destroySymbol, nullptr);

  auto createCamera = reinterpret_cast<CreateCameraFn>(createSymbol);
  auto destroyCamera = reinterpret_cast<DestroyCameraFn>(destroySymbol);

  ICamera* camera = createCamera();
  ASSERT_NE(camera, nullptr);

  camera->setResolution(1920, 1080);
  const Math::Ray ray = camera->getRay(0.5, 0.5);

  EXPECT_DOUBLE_EQ(ray.getOrigin().x, 0.0);
  EXPECT_DOUBLE_EQ(ray.getOrigin().y, 0.0);
  EXPECT_DOUBLE_EQ(ray.getOrigin().z, 0.0);
  EXPECT_DOUBLE_EQ(ray.getDirection().x, 0.0);
  EXPECT_DOUBLE_EQ(ray.getDirection().y, 0.0);
  EXPECT_DOUBLE_EQ(ray.getDirection().z, -1.0);

  destroyCamera(camera);
  ASSERT_EQ(dlclose(handle), 0);
}
