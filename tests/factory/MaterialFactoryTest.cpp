/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MaterialFactory unit tests
*/

#include <gtest/gtest.h>
#include <libconfig.h++>
#include <memory>
#include "exceptions/Exceptions.hpp"
#include "factory/material/MaterialFactory.hpp"
#include "utils/math/Color.hpp"

using raytracer::core::factory::MaterialFactory;
using raytracer::math::Color;

namespace {

class CfgFromString {
 public:
  explicit CfgFromString(const char* source) {
    config_.readString(source);
  }

  [[nodiscard]] const libconfig::Setting& at(const char* path) const {
    return config_.lookup(path);
  }

 private:
  libconfig::Config config_;
};

}  // namespace

// Given: a default call to createDiffuse.
// When:  no parameters are passed.
// Then:  the factory returns a non-null IMaterial that does not emit light.
TEST(MaterialFactoryTest, CreateDiffuseWithDefaultsReturnsNonEmissive) {
  auto material = MaterialFactory::createDiffuse();
  ASSERT_NE(material, nullptr);
  const auto emitted = material->emitted();
  EXPECT_DOUBLE_EQ(emitted.r, 0.0);
  EXPECT_DOUBLE_EQ(emitted.g, 0.0);
  EXPECT_DOUBLE_EQ(emitted.b, 0.0);
}

// Given: a libconfig setting for a diffuse material with an albedo block.
// When:  create("diffuse", cfg) is called.
// Then:  the resulting material is non-null.
TEST(MaterialFactoryTest, CreateDiffuseFromCfg) {
  CfgFromString cfg(
      "diffuse = { albedo = { r = 200; g = 100; b = 50; }; };");
  auto material = MaterialFactory::create("diffuse", cfg.at("diffuse"));
  EXPECT_NE(material, nullptr);
}

// Given: a libconfig setting for a glossy material.
// When:  create("glossy", cfg) is called.
// Then:  the resulting material is non-null.
TEST(MaterialFactoryTest, CreateGlossyFromCfg) {
  CfgFromString cfg(
      "glossy = { fuzz = 0.3; albedo = { r = 255; g = 255; b = 255; }; };");
  auto material = MaterialFactory::create("glossy", cfg.at("glossy"));
  EXPECT_NE(material, nullptr);
}

// Given: a libconfig setting for a glass material with a custom IOR.
// When:  create("glass", cfg) is called.
// Then:  the resulting material is non-null.
TEST(MaterialFactoryTest, CreateGlassFromCfg) {
  CfgFromString cfg("glass = { refractionIndex = 1.33; };");
  auto material = MaterialFactory::create("glass", cfg.at("glass"));
  EXPECT_NE(material, nullptr);
}

// Given: an empty libconfig setting.
// When:  create("glass", cfg) is called.
// Then:  the resulting material is non-null (defaults applied).
TEST(MaterialFactoryTest, CreateGlassFromEmptyCfgFallsBackToDefaults) {
  CfgFromString cfg("glass = { };");
  auto material = MaterialFactory::create("glass", cfg.at("glass"));
  EXPECT_NE(material, nullptr);
}

// Given: an unknown type name.
// When:  create() is called.
// Then:  it throws RaytracerException.
TEST(MaterialFactoryTest, CreateUnknownTypeThrows) {
  CfgFromString cfg("entry = { };");
  EXPECT_THROW(
      static_cast<void>(MaterialFactory::create("plastic", cfg.at("entry"))),
      raytracer::core::RaytracerException);
}
