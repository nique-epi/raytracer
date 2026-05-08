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

class CylinderPluginFixture : public PrimitivePluginFixture {
 public:
  CylinderPluginFixture() : PrimitivePluginFixture("Cylinder") {}

 protected:
  std::shared_ptr<IObject> makeCylinder() { return makePrimitive("cylinder"); }
};
