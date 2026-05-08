/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Plane plugin test fixture
*/

#pragma once

#include <memory>
#include "components/Primitives/IObject.hpp"
#include "fixtures/PrimitivePluginFixture.hpp"

class PlanePluginFixture : public PrimitivePluginFixture {
 public:
  PlanePluginFixture() : PrimitivePluginFixture("Plane") {}

 protected:
  std::shared_ptr<IObject> makePlane() { return makePrimitive("plane"); }
};
