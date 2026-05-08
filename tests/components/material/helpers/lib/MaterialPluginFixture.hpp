/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MaterialPluginFixture
*/

#pragma once

#include <gtest/gtest.h>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "components/material/IMaterial.hpp"
#include "helpers/lib/PluginHandle.hpp"

namespace raytracer::tests::helpers {

/**
 * @brief Shared test fixture for material plugins.
 *
 * The fixture loads the plugin with @ref PluginHandle, resolves the
 * `createMaterial` / `destroyMaterial` entry points, and keeps track of
 * created material instances so they are released automatically in
 * TearDown().
 *
 * Subclasses only need to provide the plugin file name and the expected
 * `createMaterial` signature.
 */
template <typename CreateFn>
class MaterialPluginFixture : public ::testing::Test {
 protected:
  using DestroyMaterialFn = void (*)(IMaterial*);

  void SetUp() override {
    plugin_.emplace(findPlugin(pluginFileName()));
    createFn_ = plugin_->resolve<CreateFn>("createMaterial");
    destroyFn_ = plugin_->resolve<DestroyMaterialFn>("destroyMaterial");
  }

  void TearDown() override {
    if (destroyFn_ != nullptr) {
      for (IMaterial* material : materials_) {
        if (material != nullptr) {
          destroyFn_(material);
        }
      }
    }
    materials_.clear();
    plugin_.reset();
  }

  /**
   * @brief Create a material instance through the plugin and remember it
   *        for automatic cleanup.
   */
  template <typename... Args>
  IMaterial* makeMaterial(Args&&... args) {
    if (createFn_ == nullptr) {
      throw std::runtime_error("createMaterial function not loaded");
    }
    IMaterial* material = createFn_(std::forward<Args>(args)...);
    if (material == nullptr) {
      throw std::runtime_error("createMaterial returned nullptr");
    }
    materials_.push_back(material);
    return material;
  }

  /**
   * @brief Name of the plugin file to load.
   */
  virtual std::string pluginFileName() const = 0;

  std::optional<PluginHandle> plugin_;
  CreateFn createFn_ = nullptr;
  DestroyMaterialFn destroyFn_ = nullptr;

 private:
  std::vector<IMaterial*> materials_;
};

}  // namespace raytracer::tests::helpers
