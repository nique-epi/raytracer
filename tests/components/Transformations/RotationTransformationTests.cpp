/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** RotationTransformationTests
*/

#include <gtest/gtest.h>
#include <memory>
#include "components/Transformations/Rotation/Rotation.hpp"
#include "utils/math/Vector3D.hpp"

using namespace raytracer::components::transformation;
using namespace raytracer::math;

class RotationTransformationTest : public ::testing::Test {
 protected:
  Rotation rotation;
};

TEST_F(RotationTransformationTest, DefaultRotationIsIdentity) {
  Vector3D point(1.0, 2.0, 3.0);
  Vector3D result = rotation.apply(point);

  EXPECT_DOUBLE_EQ(result.x, 1.0);
  EXPECT_DOUBLE_EQ(result.y, 2.0);
  EXPECT_DOUBLE_EQ(result.z, 3.0);
}

TEST_F(RotationTransformationTest, ApplyToNormalPreservesDirection) {
  Vector3D normal(1.0, 0.0, 0.0);
  normal = normal.normalize();
  Vector3D result = rotation.applyToNormal(normal);

  EXPECT_DOUBLE_EQ(result.x, 1.0);
  EXPECT_DOUBLE_EQ(result.y, 0.0);
  EXPECT_DOUBLE_EQ(result.z, 0.0);
}

TEST_F(RotationTransformationTest, InverseRotationIsValid) {
  auto inverse = rotation.inverse();
  ASSERT_NE(inverse, nullptr);

  Vector3D point(1.0, 2.0, 3.0);
  Vector3D rotated = rotation.apply(point);
  Vector3D unrotated = inverse->apply(rotated);

  EXPECT_DOUBLE_EQ(unrotated.x, point.x);
  EXPECT_DOUBLE_EQ(unrotated.y, point.y);
  EXPECT_DOUBLE_EQ(unrotated.z, point.z);
}

TEST_F(RotationTransformationTest, CopyConstructor) {
  Rotation copy = rotation;
  Vector3D point(1.0, 0.0, 0.0);

  EXPECT_DOUBLE_EQ(copy.apply(point).x, rotation.apply(point).x);
  EXPECT_DOUBLE_EQ(copy.apply(point).y, rotation.apply(point).y);
  EXPECT_DOUBLE_EQ(copy.apply(point).z, rotation.apply(point).z);
}

TEST_F(RotationTransformationTest, CopyAssignmentOperator) {
  Rotation copy;
  copy = rotation;
  Vector3D point(1.0, 0.0, 0.0);

  EXPECT_DOUBLE_EQ(copy.apply(point).x, rotation.apply(point).x);
  EXPECT_DOUBLE_EQ(copy.apply(point).y, rotation.apply(point).y);
  EXPECT_DOUBLE_EQ(copy.apply(point).z, rotation.apply(point).z);
}

TEST_F(RotationTransformationTest, MoveConstructor) {
  Rotation original;
  Rotation moved = std::move(original);
  Vector3D point(1.0, 0.0, 0.0);

  EXPECT_DOUBLE_EQ(moved.apply(point).x, 1.0);
  EXPECT_DOUBLE_EQ(moved.apply(point).y, 0.0);
  EXPECT_DOUBLE_EQ(moved.apply(point).z, 0.0);
}

TEST_F(RotationTransformationTest, MoveAssignmentOperator) {
  Rotation original;
  Rotation moved;
  moved = std::move(original);
  Vector3D point(1.0, 0.0, 0.0);

  EXPECT_DOUBLE_EQ(moved.apply(point).x, 1.0);
  EXPECT_DOUBLE_EQ(moved.apply(point).y, 0.0);
  EXPECT_DOUBLE_EQ(moved.apply(point).z, 0.0);
}
