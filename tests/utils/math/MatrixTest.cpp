/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Matrix unit tests
*/

#include <gtest/gtest.h>
#include <numbers>
#include "utils/math/Matrix.hpp"

using raytracer::math::Matrix4x4;
using raytracer::math::Vector3D;

namespace {
constexpr double EPS = 1e-9;
constexpr double ROTATION_EPS = 1e-6;
}  // namespace

TEST(MatrixTest, IdentityLeavesPointUnchanged) {
  const Matrix4x4 matrix = Matrix4x4::identity();
  const Vector3D point(1.5, -2.0, 4.25);

  const Vector3D transformed = matrix.transformPoint(point);

  EXPECT_NEAR(transformed.x, point.x, EPS);
  EXPECT_NEAR(transformed.y, point.y, EPS);
  EXPECT_NEAR(transformed.z, point.z, EPS);
}

TEST(MatrixTest, TranslationMovesPointsOnly) {
  const Matrix4x4 matrix = Matrix4x4::translation(Vector3D(3.0, -4.0, 5.0));
  const Vector3D point(1.0, 2.0, 3.0);
  const Vector3D direction(1.0, 2.0, 3.0);

  const Vector3D transformedPoint = matrix.transformPoint(point);
  const Vector3D transformedDirection = matrix.transformDirection(direction);

  EXPECT_NEAR(transformedPoint.x, 4.0, EPS);
  EXPECT_NEAR(transformedPoint.y, -2.0, EPS);
  EXPECT_NEAR(transformedPoint.z, 8.0, EPS);
  EXPECT_NEAR(transformedDirection.x, direction.x, EPS);
  EXPECT_NEAR(transformedDirection.y, direction.y, EPS);
  EXPECT_NEAR(transformedDirection.z, direction.z, EPS);
}

TEST(MatrixTest, ScalingAffectsPointsAndDirections) {
  const Matrix4x4 matrix = Matrix4x4::scaling(Vector3D(2.0, 3.0, 4.0));
  const Vector3D input(1.0, -2.0, 0.5);

  const Vector3D transformedPoint = matrix.transformPoint(input);
  const Vector3D transformedDirection = matrix.transformDirection(input);

  EXPECT_NEAR(transformedPoint.x, 2.0, EPS);
  EXPECT_NEAR(transformedPoint.y, -6.0, EPS);
  EXPECT_NEAR(transformedPoint.z, 2.0, EPS);
  EXPECT_NEAR(transformedDirection.x, 2.0, EPS);
  EXPECT_NEAR(transformedDirection.y, -6.0, EPS);
  EXPECT_NEAR(transformedDirection.z, 2.0, EPS);
}

TEST(MatrixTest, RotationAroundZAxis) {
  const Matrix4x4 matrix =
      Matrix4x4::rotation(static_cast<float>(std::numbers::pi_v<double> / 2.0),
                          Vector3D(0.0, 0.0, 1.0));

  const Vector3D transformed =
      matrix.transformDirection(Vector3D(1.0, 0.0, 0.0));

  EXPECT_NEAR(transformed.x, 0.0, ROTATION_EPS);
  EXPECT_NEAR(transformed.y, 1.0, ROTATION_EPS);
  EXPECT_NEAR(transformed.z, 0.0, ROTATION_EPS);
}

TEST(MatrixTest, TranslationInverseRestoresPoint) {
  const Matrix4x4 matrix = Matrix4x4::translation(Vector3D(7.0, -3.0, 2.0));
  const Matrix4x4 inverse = matrix.inverse();
  const Vector3D point(1.0, 2.0, 3.0);

  const Vector3D transformed =
      inverse.transformPoint(matrix.transformPoint(point));

  EXPECT_NEAR(transformed.x, point.x, EPS);
  EXPECT_NEAR(transformed.y, point.y, EPS);
  EXPECT_NEAR(transformed.z, point.z, EPS);
}

TEST(MatrixTest, ScalingInverseRestoresDirection) {
  const Matrix4x4 matrix = Matrix4x4::scaling(Vector3D(2.0, 4.0, 8.0));
  const Matrix4x4 inverse = matrix.inverse();
  const Vector3D direction(3.0, -2.0, 1.0);

  const Vector3D transformed =
      inverse.transformDirection(matrix.transformDirection(direction));

  EXPECT_NEAR(transformed.x, direction.x, EPS);
  EXPECT_NEAR(transformed.y, direction.y, EPS);
  EXPECT_NEAR(transformed.z, direction.z, EPS);
}

TEST(MatrixTest, SingularMatrixInverseThrows) {
  const Matrix4x4 matrix;
  const Matrix4x4 clearMatrix;

  EXPECT_EQ(matrix.inverse(), clearMatrix);
}
