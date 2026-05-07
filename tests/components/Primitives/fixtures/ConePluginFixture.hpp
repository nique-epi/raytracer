/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Cone plugin test fixture
*/

#pragma once

#include <memory>
#include "components/Primitives/IObject.hpp"
#include "fixtures/PrimitivePluginFixture.hpp"

class ConePluginFixture : public PrimitivePluginFixture {
 public:
  ConePluginFixture() : PrimitivePluginFixture("Cone") {}

 protected:
  std::shared_ptr<IObject> makeCone() { return makePrimitive("cone"); }
};
