/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Diffuse material plugin test fixture
*/

#pragma once

#include "components/material/helpers/lib/MaterialPluginFixture.hpp"
#include "utils/math/Color.hpp"

class DiffusePluginFixture
    : public raytracer::tests::helpers::MaterialPluginFixture<
          IMaterial* (*)(const raytracer::math::Color&)> {
 protected:
  using CreateMaterialFn = IMaterial* (*)(const raytracer::math::Color&);
  using DestroyMaterialFn = void (*)(IMaterial*);

  void SetUp() override {
    raytracer::tests::helpers::MaterialPluginFixture<CreateMaterialFn>::SetUp();
    const raytracer::math::Color expectedAlbedo(1.0, 1.0, 1.0);
    material = makeMaterial(expectedAlbedo);
  }

  void TearDown() override {
    material = nullptr;
    raytracer::tests::helpers::MaterialPluginFixture<
        CreateMaterialFn>::TearDown();
  }

  std::string pluginFileName() const override { return "DiffuseMaterial.so"; }

  IMaterial* material = nullptr;
};
