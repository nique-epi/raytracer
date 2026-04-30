/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Registry test fixture
*/

#pragma once

#include <gtest/gtest.h>
#include <libconfig.h++>
#include "components/Primitive/IObject.hpp"

class DummyObject final : public IObject {
 public:
  explicit DummyObject(int v) : value(v) {}

  bool hits(const raytracer::math::Ray& /*ray*/, double /*tMin*/,
            double /*tMax*/,
            raytracer::math::HitRecord& /*rec*/) const override {
    return false;
  }

  [[nodiscard]] raytracer::math::AABB getBoundingBox() const override {
    return {};
  }

  void applyTransformation(const ITransformation& /*transform*/) override {}

  int value;
};

class RegistryFixture : public ::testing::Test {
 protected:
  void SetUp() override {
    cfg.getRoot().add("stub", libconfig::Setting::TypeGroup);
    stubSetting = &cfg.getRoot()["stub"];
  }

  libconfig::Config cfg;
  libconfig::Setting* stubSetting = nullptr;
};
