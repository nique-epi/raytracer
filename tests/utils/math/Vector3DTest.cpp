/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Vector3D unit tests
*/

#include <gtest/gtest.h>
#include <cmath>
#include "core/Exceptions.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::math::Vector3D;

TEST(Vector3DTest, DefaultConstructorIsZero) {
  Vector3D v;
  EXPECT_DOUBLE_EQ(v.x, 0.0);
  EXPECT_DOUBLE_EQ(v.y, 0.0);
  EXPECT_DOUBLE_EQ(v.z, 0.0);
}

TEST(Vector3DTest, ParameterizedConstructor) {
  Vector3D v(1.0, 2.0, 3.0);
  EXPECT_DOUBLE_EQ(v.x, 1.0);
  EXPECT_DOUBLE_EQ(v.y, 2.0);
  EXPECT_DOUBLE_EQ(v.z, 3.0);
}

TEST(Vector3DTest, LengthSquared) {
  Vector3D v(1.0, 2.0, 2.0);
  EXPECT_DOUBLE_EQ(v.lengthSquared(), 9.0);
}

TEST(Vector3DTest, Length) {
  Vector3D v(1.0, 2.0, 2.0);
  EXPECT_DOUBLE_EQ(v.length(), 3.0);
}

TEST(Vector3DTest, LengthZeroVector) {
  Vector3D v;
  EXPECT_DOUBLE_EQ(v.length(), 0.0);
}

TEST(Vector3DTest, Normalize) {
  Vector3D v(0.0, 3.0, 0.0);
  Vector3D n = v.normalize();
  EXPECT_DOUBLE_EQ(n.x, 0.0);
  EXPECT_DOUBLE_EQ(n.y, 1.0);
  EXPECT_DOUBLE_EQ(n.z, 0.0);
  EXPECT_DOUBLE_EQ(n.length(), 1.0);
}

TEST(Vector3DTest, NormalizeZeroVectorThrows) {
  Vector3D v;
  EXPECT_THROW(
      {
        auto _ = v.normalize();
        (void)_;
      },
      raytracer::core::RaytracerException);
}

TEST(Vector3DTest, Dot) {
  Vector3D a(1.0, 2.0, 3.0);
  Vector3D b(4.0, 5.0, 6.0);
  EXPECT_DOUBLE_EQ(a.dot(b), 32.0);
}

TEST(Vector3DTest, DotOrthogonal) {
  Vector3D a(1.0, 0.0, 0.0);
  Vector3D b(0.0, 1.0, 0.0);
  EXPECT_DOUBLE_EQ(a.dot(b), 0.0);
}

TEST(Vector3DTest, CrossProduct) {
  Vector3D a(1.0, 0.0, 0.0);
  Vector3D b(0.0, 1.0, 0.0);
  Vector3D c = a.cross(b);
  EXPECT_DOUBLE_EQ(c.x, 0.0);
  EXPECT_DOUBLE_EQ(c.y, 0.0);
  EXPECT_DOUBLE_EQ(c.z, 1.0);
}

TEST(Vector3DTest, CrossCollinearIsZero) {
  Vector3D a(1.0, 2.0, 3.0);
  Vector3D b(2.0, 4.0, 6.0);
  Vector3D c = a.cross(b);
  EXPECT_DOUBLE_EQ(c.x, 0.0);
  EXPECT_DOUBLE_EQ(c.y, 0.0);
  EXPECT_DOUBLE_EQ(c.z, 0.0);
}

TEST(Vector3DTest, AddOperator) {
  Vector3D a(1.0, 2.0, 3.0);
  Vector3D b(4.0, 5.0, 6.0);
  Vector3D c = a + b;
  EXPECT_DOUBLE_EQ(c.x, 5.0);
  EXPECT_DOUBLE_EQ(c.y, 7.0);
  EXPECT_DOUBLE_EQ(c.z, 9.0);
}

TEST(Vector3DTest, SubtractOperator) {
  Vector3D a(4.0, 5.0, 6.0);
  Vector3D b(1.0, 2.0, 3.0);
  Vector3D c = a - b;
  EXPECT_DOUBLE_EQ(c.x, 3.0);
  EXPECT_DOUBLE_EQ(c.y, 3.0);
  EXPECT_DOUBLE_EQ(c.z, 3.0);
}

TEST(Vector3DTest, UnaryNegate) {
  Vector3D a(1.0, -2.0, 3.0);
  Vector3D b = -a;
  EXPECT_DOUBLE_EQ(b.x, -1.0);
  EXPECT_DOUBLE_EQ(b.y, 2.0);
  EXPECT_DOUBLE_EQ(b.z, -3.0);
}

TEST(Vector3DTest, ScalarMultiply) {
  Vector3D a(1.0, 2.0, 3.0);
  Vector3D b = a * 2.0;
  EXPECT_DOUBLE_EQ(b.x, 2.0);
  EXPECT_DOUBLE_EQ(b.y, 4.0);
  EXPECT_DOUBLE_EQ(b.z, 6.0);
}

TEST(Vector3DTest, ScalarMultiplyCommutative) {
  Vector3D a(1.0, 2.0, 3.0);
  Vector3D b = 2.0 * a;
  EXPECT_DOUBLE_EQ(b.x, 2.0);
  EXPECT_DOUBLE_EQ(b.y, 4.0);
  EXPECT_DOUBLE_EQ(b.z, 6.0);
}

TEST(Vector3DTest, ComponentWiseMultiply) {
  Vector3D a(1.0, 2.0, 3.0);
  Vector3D b(2.0, 3.0, 4.0);
  Vector3D c = a * b;
  EXPECT_DOUBLE_EQ(c.x, 2.0);
  EXPECT_DOUBLE_EQ(c.y, 6.0);
  EXPECT_DOUBLE_EQ(c.z, 12.0);
}

TEST(Vector3DTest, ScalarDivide) {
  Vector3D a(2.0, 4.0, 6.0);
  Vector3D b = a / 2.0;
  EXPECT_DOUBLE_EQ(b.x, 1.0);
  EXPECT_DOUBLE_EQ(b.y, 2.0);
  EXPECT_DOUBLE_EQ(b.z, 3.0);
}

TEST(Vector3DTest, AddAssign) {
  Vector3D a(1.0, 2.0, 3.0);
  a += Vector3D(1.0, 1.0, 1.0);
  EXPECT_DOUBLE_EQ(a.x, 2.0);
  EXPECT_DOUBLE_EQ(a.y, 3.0);
  EXPECT_DOUBLE_EQ(a.z, 4.0);
}

TEST(Vector3DTest, SubtractAssign) {
  Vector3D a(3.0, 3.0, 3.0);
  a -= Vector3D(1.0, 1.0, 1.0);
  EXPECT_DOUBLE_EQ(a.x, 2.0);
  EXPECT_DOUBLE_EQ(a.y, 2.0);
  EXPECT_DOUBLE_EQ(a.z, 2.0);
}

TEST(Vector3DTest, MultiplyAssign) {
  Vector3D a(1.0, 2.0, 3.0);
  a *= 3.0;
  EXPECT_DOUBLE_EQ(a.x, 3.0);
  EXPECT_DOUBLE_EQ(a.y, 6.0);
  EXPECT_DOUBLE_EQ(a.z, 9.0);
}

TEST(Vector3DTest, DivideAssign) {
  Vector3D a(3.0, 6.0, 9.0);
  a /= 3.0;
  EXPECT_DOUBLE_EQ(a.x, 1.0);
  EXPECT_DOUBLE_EQ(a.y, 2.0);
  EXPECT_DOUBLE_EQ(a.z, 3.0);
}

TEST(Vector3DTest, Point3DAliasIsVector3D) {
  raytracer::math::Point3D p(1.0, 2.0, 3.0);
  Vector3D v = p;
  EXPECT_DOUBLE_EQ(v.x, p.x);
}
