/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** LightFactory unit tests
*/

#include <gtest/gtest.h>
#include <libconfig.h++>
#include <memory>
#include "exceptions/Exceptions.hpp"
#include "factory/LightFactory.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::core::factory::LightFactory;
using raytracer::math::Color;
using raytracer::math::Vector3D;

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

// Given: default-constructed call to createAmbient.
// When:  no parameters are passed.
// Then:  the light returns intensity 1.0 and a non-null shared_ptr.
TEST(LightFactoryTest, CreateAmbientWithDefaultsReturnsUnitIntensity) {
  auto light = LightFactory::createAmbient();
  ASSERT_NE(light, nullptr);
  EXPECT_DOUBLE_EQ(light->getIntensity(), 1.0);
}

// Given: a libconfig setting with explicit color and intensity.
// When:  create("ambient", cfg) is called.
// Then:  the resulting light reflects the requested intensity.
TEST(LightFactoryTest, CreateAmbientFromCfgReadsIntensity) {
  CfgFromString cfg(
      "ambient = { color = { r = 255; g = 255; b = 255; }; intensity = 0.3; };");
  auto light = LightFactory::create("ambient", cfg.at("ambient"));
  ASSERT_NE(light, nullptr);
  EXPECT_DOUBLE_EQ(light->getIntensity(), 0.3);
}

// Given: an empty libconfig setting.
// When:  create("ambient", cfg) is called.
// Then:  the light falls back to default intensity 1.0.
TEST(LightFactoryTest, CreateAmbientFromEmptyCfgFallsBackToDefaults) {
  CfgFromString cfg("ambient = { };");
  auto light = LightFactory::create("ambient", cfg.at("ambient"));
  ASSERT_NE(light, nullptr);
  EXPECT_DOUBLE_EQ(light->getIntensity(), 1.0);
}

// Given: a libconfig setting describing a directional light.
// When:  create("directional", cfg) is called.
// Then:  the resulting light is non-null and reports the requested intensity.
TEST(LightFactoryTest, CreateDirectionalFromCfgReadsIntensity) {
  CfgFromString cfg(
      "directional = { direction = { x = 1.0; y = -1.0; z = -1.0; };"
      " color = { r = 255; g = 255; b = 255; }; intensity = 0.8; };");
  auto light = LightFactory::create("directional", cfg.at("directional"));
  ASSERT_NE(light, nullptr);
  EXPECT_DOUBLE_EQ(light->getIntensity(), 0.8);
}

// Given: a libconfig setting describing a directional light with a known direction.
// When:  create("directional", cfg) is called.
// Then:  getDirection returns the unit-length form of the requested vector.
TEST(LightFactoryTest, CreateDirectionalNormalisesDirection) {
  CfgFromString cfg(
      "directional = { direction = { x = 0.0; y = -2.0; z = 0.0; }; };");
  auto light = LightFactory::create("directional", cfg.at("directional"));
  ASSERT_NE(light, nullptr);
  const auto dir = light->getDirection({0.0, 0.0, 0.0});
  EXPECT_DOUBLE_EQ(dir.x, 0.0);
  EXPECT_DOUBLE_EQ(dir.y, -1.0);
  EXPECT_DOUBLE_EQ(dir.z, 0.0);
}

// Given: an unknown type name.
// When:  create() is called.
// Then:  it throws RaytracerException.
TEST(LightFactoryTest, CreateUnknownTypeThrows) {
  CfgFromString cfg("entry = { };");
  EXPECT_THROW(LightFactory::create("spotlight", cfg.at("entry")),
               raytracer::core::RaytracerException);
}
