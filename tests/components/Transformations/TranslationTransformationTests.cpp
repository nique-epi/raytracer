/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TranslationTransformationTests
*/

#include <gtest/gtest.h>
#include <memory>
#include "components/Transformations/Translation/Translation.hpp"
#include "utils/math/Vector3D.hpp"

using namespace raytracer::components::transformation;
using namespace raytracer::math;

class TranslationTransformationTest : public ::testing::Test {
 protected:
  Translation translation;
};

TEST_F(TranslationTransformationTest, DefaultTranslationIsIdentity) {
  Vector3D point(1.0, 2.0, 3.0);
  Vector3D result = translation.apply(point);

  EXPECT_DOUBLE_EQ(result.x, 1.0);
  EXPECT_DOUBLE_EQ(result.y, 2.0);
  EXPECT_DOUBLE_EQ(result.z, 3.0);
}

TEST_F(TranslationTransformationTest, ApplyToNormalReturnsNormal) {
  Vector3D normal(1.0, 0.0, 0.0);
  Vector3D result = translation.applyToNormal(normal);

  EXPECT_DOUBLE_EQ(result.x, 1.0);
  EXPECT_DOUBLE_EQ(result.y, 0.0);
  EXPECT_DOUBLE_EQ(result.z, 0.0);
}

TEST_F(TranslationTransformationTest, InverseTranslationIsValid) {
  auto inverse = translation.inverse();
  ASSERT_NE(inverse, nullptr);

  Vector3D point(1.0, 2.0, 3.0);
  Vector3D translated = translation.apply(point);
  Vector3D untranslated = inverse->apply(translated);

  EXPECT_DOUBLE_EQ(untranslated.x, point.x);
  EXPECT_DOUBLE_EQ(untranslated.y, point.y);
  EXPECT_DOUBLE_EQ(untranslated.z, point.z);
}

TEST_F(TranslationTransformationTest, CopyConstructor) {
  Translation copy = translation;
  Vector3D point(1.0, 2.0, 3.0);

  EXPECT_DOUBLE_EQ(copy.apply(point).x, translation.apply(point).x);
  EXPECT_DOUBLE_EQ(copy.apply(point).y, translation.apply(point).y);
  EXPECT_DOUBLE_EQ(copy.apply(point).z, translation.apply(point).z);
}

TEST_F(TranslationTransformationTest, CopyAssignmentOperator) {
  Translation copy;
  copy = translation;
  Vector3D point(1.0, 2.0, 3.0);

  EXPECT_DOUBLE_EQ(copy.apply(point).x, translation.apply(point).x);
  EXPECT_DOUBLE_EQ(copy.apply(point).y, translation.apply(point).y);
  EXPECT_DOUBLE_EQ(copy.apply(point).z, translation.apply(point).z);
}

TEST_F(TranslationTransformationTest, MoveConstructor) {
  Translation original;
  Translation moved = std::move(original);
  Vector3D point(1.0, 2.0, 3.0);

  EXPECT_DOUBLE_EQ(moved.apply(point).x, 1.0);
  EXPECT_DOUBLE_EQ(moved.apply(point).y, 2.0);
  EXPECT_DOUBLE_EQ(moved.apply(point).z, 3.0);
}

TEST_F(TranslationTransformationTest, MoveAssignmentOperator) {
  Translation original;
  Translation moved;
  moved = std::move(original);
  Vector3D point(1.0, 2.0, 3.0);

  EXPECT_DOUBLE_EQ(moved.apply(point).x, 1.0);
  EXPECT_DOUBLE_EQ(moved.apply(point).y, 2.0);
  EXPECT_DOUBLE_EQ(moved.apply(point).z, 3.0);
}
