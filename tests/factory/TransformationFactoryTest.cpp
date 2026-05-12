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
#include "factory/transformation/TransformationFactory.hpp"
#include "helpers/CfgFromString.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::core::factory::TransformationFactory;
using raytracer::math::Vector3D;
using raytracer::tests::CfgFromString;

TEST(TransformationFactoryTest, DefaultTranslationIsIdentity) {
  auto translation = TransformationFactory::createTranslation();
  ASSERT_NE(translation, nullptr);
  const Vector3D point{1.0, 2.0, 3.0};
  const auto applied = translation->apply(point);
  EXPECT_DOUBLE_EQ(applied.x, point.x);
  EXPECT_DOUBLE_EQ(applied.y, point.y);
  EXPECT_DOUBLE_EQ(applied.z, point.z);
}

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

TEST(TransformationFactoryTest, CreateRotationFromCfgReturnsNonNull) {
  CfgFromString cfg(
      "rotation = { axis = { x = 0.0; y = 0.0; z = 1.0; }; angle = 0.0; };");
  auto r = TransformationFactory::create("rotation", cfg.at("rotation"));
  EXPECT_NE(r, nullptr);
}

TEST(TransformationFactoryTest, CreateUnknownTypeThrows) {
  CfgFromString cfg("entry = { };");
  EXPECT_THROW(static_cast<void>(
                   TransformationFactory::create("scale", cfg.at("entry"))),
               raytracer::core::RaytracerException);
}
