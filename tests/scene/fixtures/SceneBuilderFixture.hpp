/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneBuilder test fixture
*/

#pragma once

#include <gtest/gtest.h>
#include <libconfig.h++>
#include "components/Primitives/IObject.hpp"
#include "core/registry/registry.hpp"
#include "scene/Scene.hpp"

class DummyObject final : public IObject {
 public:
  bool hits(const raytracer::math::Ray&, double, double,
            raytracer::math::HitRecord&) const override;
  [[nodiscard]] raytracer::math::AABB getBoundingBox() const override;
  void applyTransformation(const ITransformation&) override;
};

class SceneBuilderFixture : public ::testing::Test {
 protected:
  void SetUp() override;

  raytracer::core::registry::ObjectRegistry objectRegistry;
  raytracer::core::registry::LightRegistry lightRegistry;
  raytracer::core::registry::CameraRegistry cameraRegistry;
  raytracer::core::registry::MaterialRegistry materialRegistry;

  libconfig::Config cfg;
  libconfig::Setting* stubSetting = nullptr;
};
