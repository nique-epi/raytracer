/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Glass material plugin test fixture
*/

#pragma once

#include <string>
#include "components/material/helpers/lib/MaterialPluginFixture.hpp"

class GlassPluginFixture
    : public raytracer::tests::helpers::MaterialPluginFixture<
          IMaterial* (*)(double)> {
 protected:
  std::string pluginFileName() const override { return "GlassMaterial.so"; }
};
