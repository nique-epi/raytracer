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
#include <stdexcept>
#include <string>
#include "components/Primitives/IObject.hpp"
#include "core/registry/registry.hpp"

class PrimitivePluginFixture : public ::testing::Test {
 protected:
  explicit PrimitivePluginFixture(std::string pluginName)
      : pluginName_(std::move(pluginName)) {}

  void SetUp() override {
    const auto candidate =
        std::filesystem::current_path() / (pluginName_ + ".so");
    if (!std::filesystem::exists(candidate)) {
      throw std::runtime_error("Could not find " + pluginName_ + ".so");
    }

    registry.loadPlugin(candidate.string());

    cfg.getRoot().add(pluginName_, libconfig::Setting::TypeGroup);
    stubSetting = &cfg.getRoot()[pluginName_];
  }

  std::shared_ptr<IObject> makePrimitive(const std::string& typeName) {
    return registry.create(typeName, *stubSetting);
  }

 protected:
  raytracer::core::registry::Registry<IObject> registry;
  libconfig::Config cfg;
  libconfig::Setting* stubSetting = nullptr;

 private:
  std::string pluginName_;
};
