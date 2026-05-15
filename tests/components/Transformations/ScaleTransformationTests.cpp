/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ScaleTransformationTests
*/

#include <gtest/gtest.h>
#include <cmath>
#include <memory>
#include "components/Transformations/Scale/Scale.hpp"
#include "utils/math/Vector3D.hpp"

using namespace raytracer::components::transformation;
using namespace raytracer::math;

class ScaleTransformationTest : public ::testing::Test {
 protected:
  Scale scale;
};

TEST_F(ScaleTransformationTest, DefaultScaleIsIdentity) {
  const Vector3D point(2.0, 3.0, 4.0);
  const Vector3D result = scale.apply(point);

  EXPECT_DOUBLE_EQ(result.x, 2.0);
  EXPECT_DOUBLE_EQ(result.y, 3.0);
  EXPECT_DOUBLE_EQ(result.z, 4.0);
}

TEST_F(ScaleTransformationTest, ConstructorInitializesFactor) {
  const Scale scaled(Vector3D(2.0, 3.0, 4.0));
  const Vector3D result = scaled.apply(Vector3D(1.0, 1.0, 1.0));

  EXPECT_DOUBLE_EQ(result.x, 2.0);
  EXPECT_DOUBLE_EQ(result.y, 3.0);
  EXPECT_DOUBLE_EQ(result.z, 4.0);
}

TEST_F(ScaleTransformationTest, SetterUpdatesFactor) {
  scale.setFactor(Vector3D(2.0, 0.5, 1.0));
  const Vector3D result = scale.apply(Vector3D(4.0, 6.0, 3.0));

  EXPECT_DOUBLE_EQ(result.x, 8.0);
  EXPECT_DOUBLE_EQ(result.y, 3.0);
  EXPECT_DOUBLE_EQ(result.z, 3.0);
}

TEST_F(ScaleTransformationTest, ApplyScalesEachAxisIndependently) {
  const Scale nonUniform(Vector3D(1.0, 2.0, 3.0));
  const Vector3D result = nonUniform.apply(Vector3D(5.0, 5.0, 5.0));

  EXPECT_DOUBLE_EQ(result.x, 5.0);
  EXPECT_DOUBLE_EQ(result.y, 10.0);
  EXPECT_DOUBLE_EQ(result.z, 15.0);
}

TEST_F(ScaleTransformationTest, ApplyToNormalUsesTransposeInverse) {
  const Scale nonUniform(Vector3D(2.0, 1.0, 1.0));
  const Vector3D normal(1.0, 0.0, 0.0);
  const Vector3D result = nonUniform.applyToNormal(normal);

  EXPECT_NEAR(result.x, 0.5 / std::sqrt(0.25), 1e-12);
  EXPECT_NEAR(result.y, 0.0, 1e-12);
  EXPECT_NEAR(result.z, 0.0, 1e-12);
  EXPECT_NEAR(result.length(), 1.0, 1e-12);
}

TEST_F(ScaleTransformationTest, ApplyToNormalResultIsNormalized) {
  const Scale nonUniform(Vector3D(3.0, 2.0, 1.0));
  const Vector3D normal(1.0, 1.0, 1.0);
  const Vector3D result = nonUniform.applyToNormal(normal);

  EXPECT_NEAR(result.length(), 1.0, 1e-12);
}

TEST_F(ScaleTransformationTest, InverseRestoresPoint) {
  const Scale scaled(Vector3D(2.0, 3.0, 4.0));
  const auto inverse = scaled.inverse();
  ASSERT_NE(inverse, nullptr);

  const Vector3D point(6.0, 9.0, 12.0);
  const Vector3D scaled_point = scaled.apply(point);
  const Vector3D restored = inverse->apply(scaled_point);

  EXPECT_NEAR(restored.x, point.x, 1e-12);
  EXPECT_NEAR(restored.y, point.y, 1e-12);
  EXPECT_NEAR(restored.z, point.z, 1e-12);
}

TEST_F(ScaleTransformationTest, InverseDefaultScaleIsIdentity) {
  const auto inverse = scale.inverse();
  ASSERT_NE(inverse, nullptr);

  const Vector3D point(1.0, 2.0, 3.0);
  const Vector3D result = inverse->apply(point);

  EXPECT_DOUBLE_EQ(result.x, 1.0);
  EXPECT_DOUBLE_EQ(result.y, 2.0);
  EXPECT_DOUBLE_EQ(result.z, 3.0);
}

TEST_F(ScaleTransformationTest, InverseGuardsAgainstZeroFactor) {
  const Scale degenerate(Vector3D(0.0, 1.0, 1.0));
  const auto inverse = degenerate.inverse();
  ASSERT_NE(inverse, nullptr);

  const Vector3D point(1.0, 1.0, 1.0);
  const Vector3D result = inverse->apply(point);

  EXPECT_TRUE(std::isfinite(result.x));
  EXPECT_TRUE(std::isfinite(result.y));
  EXPECT_TRUE(std::isfinite(result.z));
}

TEST_F(ScaleTransformationTest, CopyConstructor) {
  const Scale original(Vector3D(2.0, 3.0, 4.0));
  const Scale copy = original;
  const Vector3D point(1.0, 1.0, 1.0);

  EXPECT_DOUBLE_EQ(copy.apply(point).x, original.apply(point).x);
  EXPECT_DOUBLE_EQ(copy.apply(point).y, original.apply(point).y);
  EXPECT_DOUBLE_EQ(copy.apply(point).z, original.apply(point).z);
}

TEST_F(ScaleTransformationTest, CopyAssignmentOperator) {
  const Scale original(Vector3D(2.0, 3.0, 4.0));
  Scale copy;
  copy = original;
  const Vector3D point(1.0, 1.0, 1.0);

  EXPECT_DOUBLE_EQ(copy.apply(point).x, original.apply(point).x);
  EXPECT_DOUBLE_EQ(copy.apply(point).y, original.apply(point).y);
  EXPECT_DOUBLE_EQ(copy.apply(point).z, original.apply(point).z);
}

TEST_F(ScaleTransformationTest, MoveConstructor) {
  Scale original(Vector3D(2.0, 3.0, 4.0));
  const Scale moved = std::move(original);
  const Vector3D point(1.0, 1.0, 1.0);

  EXPECT_DOUBLE_EQ(moved.apply(point).x, 2.0);
  EXPECT_DOUBLE_EQ(moved.apply(point).y, 3.0);
  EXPECT_DOUBLE_EQ(moved.apply(point).z, 4.0);
}

TEST_F(ScaleTransformationTest, MoveAssignmentOperator) {
  Scale original(Vector3D(2.0, 3.0, 4.0));
  Scale moved;
  moved = std::move(original);
  const Vector3D point(1.0, 1.0, 1.0);

  EXPECT_DOUBLE_EQ(moved.apply(point).x, 2.0);
  EXPECT_DOUBLE_EQ(moved.apply(point).y, 3.0);
  EXPECT_DOUBLE_EQ(moved.apply(point).z, 4.0);
}
