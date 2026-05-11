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

TEST(MaterialFactoryTest, CreateDiffuseWithDefaultsReturnsNonEmissive) {
  auto material = MaterialFactory::createDiffuse();
  ASSERT_NE(material, nullptr);
  const auto emitted = material->emitted();
  EXPECT_DOUBLE_EQ(emitted.r, 0.0);
  EXPECT_DOUBLE_EQ(emitted.g, 0.0);
  EXPECT_DOUBLE_EQ(emitted.b, 0.0);
}

TEST(MaterialFactoryTest, CreateDiffuseFromCfg) {
  CfgFromString cfg(
      "diffuse = { albedo = { r = 200; g = 100; b = 50; }; };");
  auto material = MaterialFactory::create("diffuse", cfg.at("diffuse"));
  EXPECT_NE(material, nullptr);
}

TEST(MaterialFactoryTest, CreateGlossyFromCfg) {
  CfgFromString cfg(
      "glossy = { fuzz = 0.3; albedo = { r = 255; g = 255; b = 255; }; };");
  auto material = MaterialFactory::create("glossy", cfg.at("glossy"));
  EXPECT_NE(material, nullptr);
}

TEST(MaterialFactoryTest, CreateGlassFromCfg) {
  CfgFromString cfg("glass = { refractionIndex = 1.33; };");
  auto material = MaterialFactory::create("glass", cfg.at("glass"));
  EXPECT_NE(material, nullptr);
}

TEST(MaterialFactoryTest, CreateGlassFromEmptyCfgFallsBackToDefaults) {
  CfgFromString cfg("glass = { };");
  auto material = MaterialFactory::create("glass", cfg.at("glass"));
  EXPECT_NE(material, nullptr);
}

TEST(MaterialFactoryTest, CreateUnknownTypeThrows) {
  CfgFromString cfg("entry = { };");
  EXPECT_THROW(
      static_cast<void>(MaterialFactory::create("plastic", cfg.at("entry"))),
      raytracer::core::RaytracerException);
}
