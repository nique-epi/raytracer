/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Sphere plugin test fixture
*/

#pragma once

#include <gtest/gtest.h>
#include <libconfig.h++>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include "components/Primitives/IObject.hpp"
#include "core/registry/registry.hpp"

class SpherePluginFixture : public ::testing::Test {
 protected:
  void SetUp() override {
    const auto candidate = std::filesystem::current_path() / "Sphere.so";
    if (!std::filesystem::exists(candidate)) {
      throw std::runtime_error("Could not find Sphere.so");
    }

    registry.loadPlugin(candidate.string());

    cfg.getRoot().add("sphere", libconfig::Setting::TypeGroup);
    stubSetting = &cfg.getRoot()["sphere"];
  }

  std::shared_ptr<IObject> makeSphere() {
    return registry.create("sphere", *stubSetting);
  }

 protected:
  raytracer::core::registry::Registry<IObject> registry;
  libconfig::Config cfg;
  libconfig::Setting* stubSetting = nullptr;
};
