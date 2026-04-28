/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Ray unit tests
*/

#include <gtest/gtest.h>
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::math::Ray;
using raytracer::math::Vector3D;

TEST(RayTest, GetOriginAndDirection) {
  Vector3D origin(1.0, 2.0, 3.0);
  Vector3D direction(4.0, 5.0, 6.0);
  Ray r(origin, direction);
  EXPECT_DOUBLE_EQ(r.getOrigin().x, 1.0);
  EXPECT_DOUBLE_EQ(r.getOrigin().y, 2.0);
  EXPECT_DOUBLE_EQ(r.getOrigin().z, 3.0);
  EXPECT_DOUBLE_EQ(r.getDirection().x, 4.0);
  EXPECT_DOUBLE_EQ(r.getDirection().y, 5.0);
  EXPECT_DOUBLE_EQ(r.getDirection().z, 6.0);
}

TEST(RayTest, DirectionNotNormalized) {
  Vector3D origin(0.0, 0.0, 0.0);
  Vector3D direction(0.0, 0.0, 5.0);
  Ray r(origin, direction);
  EXPECT_DOUBLE_EQ(r.getDirection().length(), 5.0);
}

TEST(RayTest, AtZero) {
  Vector3D origin(1.0, 2.0, 3.0);
  Vector3D direction(4.0, 5.0, 6.0);
  Ray r(origin, direction);
  Vector3D p = r.at(0.0);
  EXPECT_DOUBLE_EQ(p.x, 1.0);
  EXPECT_DOUBLE_EQ(p.y, 2.0);
  EXPECT_DOUBLE_EQ(p.z, 3.0);
}

TEST(RayTest, AtOne) {
  Vector3D origin(1.0, 2.0, 3.0);
  Vector3D direction(4.0, 5.0, 6.0);
  Ray r(origin, direction);
  Vector3D p = r.at(1.0);
  EXPECT_DOUBLE_EQ(p.x, 5.0);
  EXPECT_DOUBLE_EQ(p.y, 7.0);
  EXPECT_DOUBLE_EQ(p.z, 9.0);
}

TEST(RayTest, AtNegative) {
  Vector3D origin(1.0, 2.0, 3.0);
  Vector3D direction(1.0, 0.0, 0.0);
  Ray r(origin, direction);
  Vector3D p = r.at(-2.0);
  EXPECT_DOUBLE_EQ(p.x, -1.0);
  EXPECT_DOUBLE_EQ(p.y, 2.0);
  EXPECT_DOUBLE_EQ(p.z, 3.0);
}
