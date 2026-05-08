/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Plane plugin test fixture
*/

#pragma once

#include <gtest/gtest.h>
#include <optional>
#include <vector>
#include "components/Primitives/IObject.hpp"
#include "helpers/lib/PluginHandle.hpp"
#include "utils/math/Vector3D.hpp"

class PlanePluginFixture : public ::testing::Test {
 protected:
  using CreatePrimitiveFn = IObject* (*)(const raytracer::math::Vector3D&,
                                         const raytracer::math::Vector3D&);
  using DestroyPrimitiveFn = void (*)(IObject*);

  void SetUp() override {
    plugin_.emplace(raytracer::tests::helpers::findPlugin("Plane.so"));
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

  IObject* makePlane(const raytracer::math::Vector3D& point,
                     const raytracer::math::Vector3D& normal) {
    IObject* obj = createFn_(point, normal);
    objects_.push_back(obj);
    return obj;
  }

  std::optional<raytracer::tests::helpers::PluginHandle> plugin_;
  CreatePrimitiveFn createFn_ = nullptr;
  DestroyPrimitiveFn destroyFn_ = nullptr;

 private:
  std::vector<IObject*> objects_;
};
