/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Cylinder plugin test fixture
*/

#pragma once

#include <gtest/gtest.h>
#include <optional>
#include <vector>
#include "components/Primitives/IObject.hpp"
#include "helpers/lib/PluginHandle.hpp"
#include "utils/math/Vector3D.hpp"

class CylinderPluginFixture : public ::testing::Test {
 protected:
  using CreatePrimitiveFn = IObject* (*)(const raytracer::math::Vector3D&,
                                         const raytracer::math::Vector3D&,
                                         double, double);
  using DestroyPrimitiveFn = void (*)(IObject*);

  void SetUp() override {
    plugin_.emplace(raytracer::tests::helpers::findPlugin("Cylinder.so"));
    createFn_ = plugin_->resolve<CreatePrimitiveFn>("createPrimitive");
    destroyFn_ = plugin_->resolve<DestroyPrimitiveFn>("destroyPrimitive");
  }

  void TearDown() override {
    for (IObject* obj : objects_) {
      if (obj != nullptr && destroyFn_ != nullptr) {
        destroyFn_(obj);
      }
    }
    objects_.clear();
    plugin_.reset();
  }

  IObject* makeCylinder(const raytracer::math::Vector3D& center,
                        const raytracer::math::Vector3D& axis,
                        double radius, double height) {
    IObject* obj = createFn_(center, axis, radius, height);
    objects_.push_back(obj);
    return obj;
  }

  std::optional<raytracer::tests::helpers::PluginHandle> plugin_;
  CreatePrimitiveFn createFn_ = nullptr;
  DestroyPrimitiveFn destroyFn_ = nullptr;

 private:
  std::vector<IObject*> objects_;
};
