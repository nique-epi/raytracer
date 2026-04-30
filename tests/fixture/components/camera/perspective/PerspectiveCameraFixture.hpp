/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Perspective camera test fixture
*/

#pragma once

#include <dlfcn.h>
#include <gtest/gtest.h>
#include <filesystem>
#include "components/camera/ICamera.hpp"

class PerspectiveCameraFixture : public ::testing::Test {
 protected:
  using CreateCameraFn = ICamera* (*)();
  using DestroyCameraFn = void (*)(ICamera*);

  void SetUp() override {
    pluginPath = findPluginPath();
    ASSERT_FALSE(pluginPath.empty())
        << "Could not find raytracer_perspective_camera.so";

    handle = dlopen(pluginPath.c_str(), RTLD_NOW | RTLD_LOCAL);
    ASSERT_NE(handle, nullptr) << dlerror();

    createCameraFn = loadSymbol<CreateCameraFn>("createCamera");
    destroyCameraFn = loadSymbol<DestroyCameraFn>("DestroyCamera");

    ASSERT_NE(createCameraFn, nullptr);
    ASSERT_NE(destroyCameraFn, nullptr);

    camera = createCameraFn();
    ASSERT_NE(camera, nullptr);
  }

  void TearDown() override {
    if (camera != nullptr && destroyCameraFn != nullptr) {
      destroyCameraFn(camera);
      camera = nullptr;
    }

    if (handle != nullptr) {
      EXPECT_EQ(dlclose(handle), 0);
      handle = nullptr;
    }
  }

  template <typename Fn>
  Fn loadSymbol(const char* symbolName) {
    dlerror();
    void* symbol = dlsym(handle, symbolName);
    EXPECT_EQ(dlerror(), nullptr) << "Failed to resolve " << symbolName;
    return reinterpret_cast<Fn>(symbol);
  }

  static std::filesystem::path findPluginPath() {
    const auto candidate =
        std::filesystem::current_path() / "raytracer_perspective_camera.so";
    if (std::filesystem::exists(candidate)) {
      return candidate;
    }

    return {};
  }

  std::filesystem::path pluginPath;
  void* handle = nullptr;
  CreateCameraFn createCameraFn = nullptr;
  DestroyCameraFn destroyCameraFn = nullptr;
  ICamera* camera = nullptr;
};
