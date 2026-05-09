/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Common primitive plugin test fixture
*/

#pragma once

#include <gtest/gtest.h>
#include <filesystem>
#include <libconfig.h++>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "components/Primitives/IObject.hpp"
#include "registry/registry.hpp"
#include "helpers/lib/PluginHandle.hpp"

class PrimitivePluginFixture : public ::testing::Test {
 protected:
  using DestroyFn = void (*)(IObject*);

  explicit PrimitivePluginFixture(std::string pluginName)
      : pluginName_(std::move(pluginName)) {}

  void SetUp() override {
    const auto candidate =
        std::filesystem::current_path() / (pluginName_ + ".so");
    if (!std::filesystem::exists(candidate)) {
      throw std::runtime_error("Could not find " + pluginName_ + ".so");
    }
    pluginPath_ = candidate;
    plugin_.emplace(candidate);
    destroyFn_ = plugin_->resolve<DestroyFn>("destroyPrimitive");
  }

  void TearDown() override {
    if (destroyFn_ != nullptr) {
      for (IObject* obj : directObjects_) {
        if (obj != nullptr) {
          destroyFn_(obj);
        }
      }
    }
    directObjects_.clear();
    plugin_.reset();
  }

  /**
   * @brief Create a primitive via the registry entry point (Sphere, Cone…).
   *
   * Uses Registry::loadPlugin() which handles the correct dlopen flags.
   * Lazy-initialised on first call — never invoked for plugins that expose
   * a direct entry point (Cylinder…).
   *
   * @param [in] typeName  Key used in @c registry.registerType (e.g. "sphere").
   */
  std::shared_ptr<IObject> makePrimitive(const std::string& typeName) {
    if (!registryLoaded_) {
      registry_.loadPlugin(pluginPath_.string());
      cfg_.getRoot().add(pluginName_, libconfig::Setting::TypeGroup);
      stubSetting_ = &cfg_.getRoot()[pluginName_];
      registryLoaded_ = true;
    }
    return registry_.create(typeName, *stubSetting_);
  }

  /**
   * @brief Create a primitive via a direct entry point with explicit args
   *        (Cylinder, Plane…).
   *
   * Resolves @c createPrimitive as @p CreateFn via the already-open
   * PluginHandle and forwards @p args to it. The returned object is tracked
   * for automatic cleanup in TearDown().
   *
   * @tparam CreateFn  Exact function pointer type of @c createPrimitive.
   * @tparam Args      Construction argument types.
   */
  template <typename CreateFn, typename... Args>
  IObject* makePrimitive(Args&&... args) {
    auto createFn = plugin_->resolve<CreateFn>("createPrimitive");
    IObject* obj = createFn(std::forward<Args>(args)...);
    if (obj == nullptr) {
      throw std::runtime_error("createPrimitive returned nullptr");
    }
    directObjects_.push_back(obj);
    return obj;
  }

 private:
  std::string pluginName_;
  std::filesystem::path pluginPath_;
  std::optional<raytracer::tests::helpers::PluginHandle> plugin_;
  DestroyFn destroyFn_ = nullptr;
  std::vector<IObject*> directObjects_;

  raytracer::core::registry::Registry<IObject> registry_;
  libconfig::Config cfg_;
  libconfig::Setting* stubSetting_ = nullptr;
  bool registryLoaded_ = false;
};
