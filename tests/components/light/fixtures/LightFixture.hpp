/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** LightFixture
*/

#pragma once

#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include "components/light/ILight.hpp"
#include "helpers/lib/PluginHandle.hpp"

/**
 * @brief Base fixture for any ILight plugin test.
 *
 * Subclasses override @ref pluginFileName to point to the .so they
 * want to exercise (e.g. "ambient.so", "directional.so"). The fixture
 * loads the plugin, resolves `createLight` / `DestroyLight`, builds an
 * ILight instance for the test, and tears everything down in the right
 * order (light first, then dlclose).
 */
class LightFixture : public ::testing::Test {
 protected:
  using CreateLightFn = ILight* (*)();
  using DestroyLightFn = void (*)(ILight*);

  void SetUp() override {
    plugin.emplace(raytracer::tests::helpers::findPlugin(pluginFileName()));
    createFn = plugin->resolve<CreateLightFn>("createLight");
    destroyFn = plugin->resolve<DestroyLightFn>("DestroyLight");
    light = createFn();
    if (light == nullptr) {
      throw std::runtime_error("createLight returned nullptr");
    }
  }

  void TearDown() override {
    if (light != nullptr && destroyFn != nullptr) {
      destroyFn(light);
      light = nullptr;
    }
    plugin.reset();
  }

  /**
   * @brief Name of the .so file this fixture should load.
   *
   * Must be defined by every concrete subclass (e.g. return
   * "ambient.so" or "directional.so").
   */
  virtual std::string pluginFileName() const = 0;

  std::optional<raytracer::tests::helpers::PluginHandle> plugin;
  CreateLightFn createFn = nullptr;
  DestroyLightFn destroyFn = nullptr;
  ILight* light = nullptr;
};
