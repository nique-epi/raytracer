/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Sphere plugin test fixture
*/

#pragma once

#include <memory>
#include "components/Primitives/IObject.hpp"
#include "fixtures/PrimitivePluginFixture.hpp"

class SpherePluginFixture : public PrimitivePluginFixture {
 public:
  SpherePluginFixture() : PrimitivePluginFixture("Sphere") {}

 protected:
  std::shared_ptr<IObject> makeSphere() { return makePrimitive("sphere"); }
};
