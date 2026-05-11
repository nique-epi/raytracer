/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PrimitiveFactory unit tests
*/

#include <gtest/gtest.h>
#include <libconfig.h++>
#include <memory>
#include "components/Primitives/IObject.hpp"
#include "exceptions/Exceptions.hpp"
#include "factory/primitive/PrimitiveFactory.hpp"
#include "utils/math/AABB.hpp"

using raytracer::core::factory::PrimitiveFactory;
using raytracer::math::Vector3D;

namespace {

class CfgFromString {
 public:
  explicit CfgFromString(const char* source) {
    config_.readString(source);
  }

  [[nodiscard]] const libconfig::Setting& root() const {
    return config_.getRoot();
  }

  [[nodiscard]] const libconfig::Setting& at(const char* path) const {
    return config_.lookup(path);
  }

 private:
  libconfig::Config config_;
};

}  // namespace

TEST(PrimitiveFactoryTest, CreateSphereWithDefaultsReturnsUnitSphereAtOrigin) {
  PrimitiveFactory factory;
  auto sphere = factory.createSphere();
  ASSERT_NE(sphere, nullptr);

  const auto box = sphere->getBoundingBox();
  EXPECT_DOUBLE_EQ(box.min.x, -1.0);
  EXPECT_DOUBLE_EQ(box.max.x, 1.0);
}

TEST(PrimitiveFactoryTest, CreateSphereParsesCenterAndRadiusFromSetting) {
  CfgFromString cfg(
      "sphere = { center = { x = 2.0; y = 3.0; z = 4.0; }; radius = 0.5; };");
  auto obj = PrimitiveFactory::create("sphere", cfg.at("sphere"));
  ASSERT_NE(obj, nullptr);

  const auto box = obj->getBoundingBox();
  EXPECT_DOUBLE_EQ(box.min.x, 1.5);
  EXPECT_DOUBLE_EQ(box.max.x, 2.5);
  EXPECT_DOUBLE_EQ(box.min.y, 2.5);
  EXPECT_DOUBLE_EQ(box.max.y, 3.5);
}

TEST(PrimitiveFactoryTest, CreateSphereWithEmptySettingFallsBackToDefaults) {
  CfgFromString cfg("sphere = { };");
  auto obj = PrimitiveFactory::create("sphere", cfg.at("sphere"));
  ASSERT_NE(obj, nullptr);

  const auto box = obj->getBoundingBox();
  EXPECT_DOUBLE_EQ(box.min.x, -1.0);
  EXPECT_DOUBLE_EQ(box.max.x, 1.0);
}

TEST(PrimitiveFactoryTest, CreateSphereWithPartialSettingKeepsDefaults) {
  CfgFromString cfg("sphere = { radius = 2.0; };");
  auto obj = PrimitiveFactory::create("sphere", cfg.at("sphere"));
  ASSERT_NE(obj, nullptr);

  const auto box = obj->getBoundingBox();
  EXPECT_DOUBLE_EQ(box.min.x, -2.0);
  EXPECT_DOUBLE_EQ(box.max.x, 2.0);
}

TEST(PrimitiveFactoryTest, CreateConeWithSettingReturnsNonNull) {
  CfgFromString cfg(
      "cone = {"
      " apex = { x = 0.0; y = 0.0; z = 0.0; };"
      " axis = { x = 0.0; y = 1.0; z = 0.0; };"
      " angle = 0.5;"
      " height = 2.0;"
      "};");
  auto obj = PrimitiveFactory::create("cone", cfg.at("cone"));
  EXPECT_NE(obj, nullptr);
}

TEST(PrimitiveFactoryTest, CreateCylinderWithSettingReturnsNonNull) {
  CfgFromString cfg(
      "cylinder = {"
      " center = { x = 0.0; y = 0.0; z = 0.0; };"
      " axis = { x = 0.0; y = 1.0; z = 0.0; };"
      " radius = 0.5;"
      " height = 1.0;"
      "};");
  auto obj = PrimitiveFactory::create("cylinder", cfg.at("cylinder"));
  EXPECT_NE(obj, nullptr);
}

TEST(PrimitiveFactoryTest, CreatePlaneWithExplicitPointAndNormal) {
  CfgFromString cfg(
      "plane = {"
      " point = { x = 0.0; y = -1.0; z = 0.0; };"
      " normal = { x = 0.0; y = 1.0; z = 0.0; };"
      "};");
  auto obj = PrimitiveFactory::create("plane", cfg.at("plane"));
  EXPECT_NE(obj, nullptr);
}

TEST(PrimitiveFactoryTest, CreatePlaneWithAxisPositionShorthand) {
  CfgFromString cfg("plane = { axis = \"Y\"; position = -1.0; };");
  auto obj = PrimitiveFactory::create("plane", cfg.at("plane"));
  EXPECT_NE(obj, nullptr);
}

TEST(PrimitiveFactoryTest, CreateUnknownTypeThrows) {
  CfgFromString cfg("entry = { };");
  EXPECT_THROW(
      static_cast<void>(PrimitiveFactory::create("torus", cfg.at("entry"))),
      raytracer::core::RaytracerException);
}
