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
#include "factory/light/LightFactory.hpp"
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

TEST(LightFactoryTest, CreateAmbientWithDefaultsReturnsUnitIntensity) {
  auto light = LightFactory::createAmbient();
  ASSERT_NE(light, nullptr);
  EXPECT_DOUBLE_EQ(light->getIntensity(), 1.0);
}

TEST(LightFactoryTest, CreateAmbientFromCfgReadsIntensity) {
  CfgFromString cfg(
      "ambient = { color = { r = 255; g = 255; b = 255; }; intensity = 0.3; };");
  auto light = LightFactory::create("ambient", cfg.at("ambient"));
  ASSERT_NE(light, nullptr);
  EXPECT_DOUBLE_EQ(light->getIntensity(), 0.3);
}

TEST(LightFactoryTest, CreateAmbientFromEmptyCfgFallsBackToDefaults) {
  CfgFromString cfg("ambient = { };");
  auto light = LightFactory::create("ambient", cfg.at("ambient"));
  ASSERT_NE(light, nullptr);
  EXPECT_DOUBLE_EQ(light->getIntensity(), 1.0);
}

TEST(LightFactoryTest, CreateDirectionalFromCfgReadsIntensity) {
  CfgFromString cfg(
      "directional = { direction = { x = 1.0; y = -1.0; z = -1.0; };"
      " color = { r = 255; g = 255; b = 255; }; intensity = 0.8; };");
  auto light = LightFactory::create("directional", cfg.at("directional"));
  ASSERT_NE(light, nullptr);
  EXPECT_DOUBLE_EQ(light->getIntensity(), 0.8);
}

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

TEST(LightFactoryTest, CreateUnknownTypeThrows) {
  CfgFromString cfg("entry = { };");
  EXPECT_THROW(
      static_cast<void>(LightFactory::create("spotlight", cfg.at("entry"))),
      raytracer::core::RaytracerException);
}
