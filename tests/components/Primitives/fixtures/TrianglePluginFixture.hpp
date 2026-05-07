/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Triangle plugin test fixture
*/

#pragma once

#include <memory>
#include "PrimitivePluginFixture.hpp"
#include "components/Primitives/IObject.hpp"

class TrianglePluginFixture : public PrimitivePluginFixture {
 public:
  TrianglePluginFixture() : PrimitivePluginFixture("Triangle") {}

 protected:
  std::shared_ptr<IObject> makeTriangle() { return makePrimitive("triangle"); }
};
