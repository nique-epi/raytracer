/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Camera plugin test fixture
*/

#pragma once

#include <dlfcn.h>
#include <gtest/gtest.h>
#include <filesystem>
#include <stdexcept>
#include <string>
#include "components/camera/ICamera.hpp"

class CameraPluginFixture : public ::testing::Test {
 protected:
  using CreateCameraFn = ICamera* (*)();
  using DestroyCameraFn = void (*)(ICamera*);

  void SetUp() override { loadPlugin(pluginFileName()); }

  void TearDown() override {
    if (camera != nullptr && destroyCameraFn != nullptr) {
      destroyCameraFn(camera);
      camera = nullptr;
    }

    if (handle != nullptr) {
      if (dlclose(handle) != 0) {
        throw std::runtime_error("Failed to close plugin handle");
      }
      handle = nullptr;
    }
  }

  virtual std::string pluginFileName() const = 0;

  void loadPlugin(const std::string& pluginName) {
    pluginPath = findPluginPath(pluginName);
    if (pluginPath.empty()) {
      throw std::runtime_error("Could not find " + pluginName);
    }

    handle = dlopen(pluginPath.c_str(), RTLD_NOW | RTLD_LOCAL);
    if (handle == nullptr) {
      const char* error = dlerror();
      throw std::runtime_error(
          makeErrorMessage("Failed to open plugin", error));
    }

    createCameraFn = loadSymbol<CreateCameraFn>("createCamera");
    destroyCameraFn = loadSymbol<DestroyCameraFn>("DestroyCamera");

    if (createCameraFn == nullptr) {
      throw std::runtime_error("Failed to resolve createCamera");
    }

    if (destroyCameraFn == nullptr) {
      throw std::runtime_error("Failed to resolve DestroyCamera");
    }

    camera = createCameraFn();
    if (camera == nullptr) {
      throw std::runtime_error("Failed to create camera");
    }
  }

  template <typename Fn>
  Fn loadSymbol(const char* symbolName) {
    dlerror();
    void* symbol = dlsym(handle, symbolName);
    const char* error = dlerror();
    if (error != nullptr || symbol == nullptr) {
      throw std::runtime_error(makeErrorMessage(
          std::string("Failed to resolve ") + symbolName, error));
    }

    return reinterpret_cast<Fn>(symbol);
  }

  static std::filesystem::path findPluginPath(const std::string& pluginName) {
    const auto candidate = std::filesystem::current_path() / pluginName;
    if (std::filesystem::exists(candidate)) {
      return candidate;
    }

    return {};
  }

  static std::string makeErrorMessage(const std::string& prefix,
                                      const char* error) {
    if (error == nullptr) {
      return prefix;
    }

    return prefix + ": " + error;
  }

  std::filesystem::path pluginPath;
  void* handle = nullptr;
  CreateCameraFn createCameraFn = nullptr;
  DestroyCameraFn destroyCameraFn = nullptr;
  ICamera* camera = nullptr;
};
