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
#include "../../fixtures/ComponentFactoryFixture.hpp"
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

  ComponentFactoryFixture factory_;

  libconfig::Config cfg_;
  libconfig::Setting* stubSetting_ = nullptr;
};
