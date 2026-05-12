/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Mesh plugin test fixture
*/

#pragma once

#include <memory>
#include "components/Primitives/IObject.hpp"
#include "PrimitivePluginFixture.hpp"

class MeshPluginFixture : public PrimitivePluginFixture {
 public:
  MeshPluginFixture() : PrimitivePluginFixture("Mesh") {}

 protected:
  std::shared_ptr<IObject> makeMesh() { return makePrimitive("mesh"); }
};
