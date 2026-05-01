/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Diffuse material plugin test fixture
*/

#pragma once

#include <dlfcn.h>
#include <gtest/gtest.h>
#include <filesystem>
#include <stdexcept>
#include <string>
#include "components/material/IMaterial.hpp"

class DiffusePluginFixture : public ::testing::Test {
 protected:
  using CreateMaterialFn = IMaterial* (*)();
  using DestroyMaterialFn = void (*)(IMaterial*);

  void SetUp() override {
    const auto candidate =
        std::filesystem::current_path() / "diffuse.so";
    if (!std::filesystem::exists(candidate)) {
      throw std::runtime_error("Could not find diffuse.so");
    }

    handle = dlopen(candidate.c_str(), RTLD_NOW | RTLD_LOCAL);
    if (handle == nullptr) {
      throw std::runtime_error(std::string("Failed to open plugin: ") +
                               dlerror());
    }

    createFn = loadSymbol<CreateMaterialFn>("createMaterial");
    destroyFn = loadSymbol<DestroyMaterialFn>("destroyMaterial");

    material = createFn();
    if (material == nullptr) {
      throw std::runtime_error("createMaterial returned nullptr");
    }
  }

  void TearDown() override {
    if (material != nullptr && destroyFn != nullptr) {
      destroyFn(material);
      material = nullptr;
    }
    if (handle != nullptr) {
      dlclose(handle);
      handle = nullptr;
    }
  }

 private:
  template <typename Fn>
  Fn loadSymbol(const char* name) {
    dlerror();
    void* sym = dlsym(handle, name);
    const char* err = dlerror();
    if (err != nullptr || sym == nullptr) {
      throw std::runtime_error(std::string("Failed to resolve ") + name +
                               (err ? std::string(": ") + err : ""));
    }
    return reinterpret_cast<Fn>(sym);
  }

 protected:
  void* handle = nullptr;
  CreateMaterialFn createFn = nullptr;
  DestroyMaterialFn destroyFn = nullptr;
  IMaterial* material = nullptr;
};
