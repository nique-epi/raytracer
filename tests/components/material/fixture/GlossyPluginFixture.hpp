/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Glossy material plugin test fixture
*/

#pragma once

#include <string>
#include "components/material/helpers/lib/MaterialPluginFixture.hpp"
#include "utils/math/Color.hpp"

class GlossyPluginFixture
    : public raytracer::tests::helpers::MaterialPluginFixture<
          IMaterial* (*)(double, const raytracer::math::Color&)> {
 protected:
  std::string pluginFileName() const override { return "GlossyMaterial.so"; }
};
