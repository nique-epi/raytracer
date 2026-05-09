/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Cylinder plugin test fixture
*/

#pragma once

#include <memory>
#include "components/Primitives/IObject.hpp"
#include "fixtures/PrimitivePluginFixture.hpp"
#include "utils/math/Vector3D.hpp"

class CylinderPluginFixture : public PrimitivePluginFixture {
 public:
  CylinderPluginFixture() : PrimitivePluginFixture("Cylinder") {}

 protected:
  using CylinderCreateFn = IObject* (*)(const raytracer::math::Vector3D&,
                                        const raytracer::math::Vector3D&,
                                        double, double);

  IObject* makeCylinder(const raytracer::math::Vector3D& center,
                        const raytracer::math::Vector3D& axis, double radius,
                        double height) {
    return makePrimitive<CylinderCreateFn>(center, axis, radius, height);
  }
};
