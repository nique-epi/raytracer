/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TransformationFactory unit tests
*/

#include <gtest/gtest.h>
#include <libconfig.h++>
#include <memory>
#include "exceptions/Exceptions.hpp"
#include "factory/TransformationFactory.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::core::factory::TransformationFactory;
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

// Given: a default call to createTranslation.
// When:  no parameters are passed.
// Then:  the factory returns the identity translation.
TEST(TransformationFactoryTest, DefaultTranslationIsIdentity) {
  auto translation = TransformationFactory::createTranslation();
  ASSERT_NE(translation, nullptr);
  const Vector3D point{1.0, 2.0, 3.0};
  const auto applied = translation->apply(point);
  EXPECT_DOUBLE_EQ(applied.x, point.x);
  EXPECT_DOUBLE_EQ(applied.y, point.y);
  EXPECT_DOUBLE_EQ(applied.z, point.z);
}

// Given: a libconfig setting describing a non-zero translation offset.
// When:  create("translation", cfg) is called.
// Then:  the resulting transformation shifts a point by the offset.
TEST(TransformationFactoryTest, CreateTranslationFromCfgAppliesOffset) {
  CfgFromString cfg(
      "translation = { offset = { x = 1.0; y = 2.0; z = 3.0; }; };");
  auto t = TransformationFactory::create("translation", cfg.at("translation"));
  ASSERT_NE(t, nullptr);
  const auto applied = t->apply({0.0, 0.0, 0.0});
  EXPECT_DOUBLE_EQ(applied.x, 1.0);
  EXPECT_DOUBLE_EQ(applied.y, 2.0);
  EXPECT_DOUBLE_EQ(applied.z, 3.0);
}

// Given: a libconfig setting for a rotation around +Z by zero.
// When:  create("rotation", cfg) is called.
// Then:  the resulting transformation is a non-null identity-like rotation.
TEST(TransformationFactoryTest, CreateRotationFromCfgReturnsNonNull) {
  CfgFromString cfg(
      "rotation = { axis = { x = 0.0; y = 0.0; z = 1.0; }; angle = 0.0; };");
  auto r = TransformationFactory::create("rotation", cfg.at("rotation"));
  EXPECT_NE(r, nullptr);
}

// Given: an unknown type name.
// When:  create() is called.
// Then:  it throws RaytracerException.
TEST(TransformationFactoryTest, CreateUnknownTypeThrows) {
  CfgFromString cfg("entry = { };");
  EXPECT_THROW(
      static_cast<void>(TransformationFactory::create("scale", cfg.at("entry"))),
      raytracer::core::RaytracerException);
}
