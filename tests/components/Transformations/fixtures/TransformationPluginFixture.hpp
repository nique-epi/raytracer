/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Transformation plugin test fixture
*/

#pragma once

#include <dlfcn.h>
#include <gtest/gtest.h>
#include <filesystem>
#include <stdexcept>
#include <string>
#include "components/Transformations/ITransformation.hpp"

class TransformationPluginFixture : public ::testing::Test {
 protected:
  using CreateTransformationFn = ITransformation* (*)();
  using DestroyTransformationFn = void (*)(ITransformation*);

  void SetUp() override { loadPlugin(pluginFileName()); }

  void TearDown() override {
    if (transformation != nullptr && destroyTransformationFn != nullptr) {
      destroyTransformationFn(transformation);
      transformation = nullptr;
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

    createTransformationFn = loadSymbol<CreateTransformationFn>("createCamera");
    destroyTransformationFn =
        loadSymbol<DestroyTransformationFn>("DestroyCamera");

    if (createTransformationFn == nullptr) {
      throw std::runtime_error("Failed to resolve createCamera");
    }

    if (destroyTransformationFn == nullptr) {
      throw std::runtime_error("Failed to resolve DestroyCamera");
    }

    transformation = createTransformationFn();
    if (transformation == nullptr) {
      throw std::runtime_error("Failed to create transformation");
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
  CreateTransformationFn createTransformationFn = nullptr;
  DestroyTransformationFn destroyTransformationFn = nullptr;
  ITransformation* transformation = nullptr;
};
