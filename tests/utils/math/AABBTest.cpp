/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** AABB unit tests
*/

#include <gtest/gtest.h>
#include "utils/math/AABB.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using Math::AABB;
using Math::Ray;
using Math::Vector3D;

TEST(AABBTest, RayHitsCenter) {
  AABB box(Vector3D(-1.0, -1.0, -1.0), Vector3D(1.0, 1.0, 1.0));
  Ray ray(Vector3D(0.0, 0.0, -5.0), Vector3D(0.0, 0.0, 1.0));
  EXPECT_TRUE(box.hit(ray, 0.0, 100.0));
}

TEST(AABBTest, RayMisses) {
  AABB box(Vector3D(-1.0, -1.0, -1.0), Vector3D(1.0, 1.0, 1.0));
  Ray ray(Vector3D(5.0, 0.0, -5.0), Vector3D(0.0, 0.0, 1.0));
  EXPECT_FALSE(box.hit(ray, 0.0, 100.0));
}

TEST(AABBTest, RayFromInside) {
  AABB box(Vector3D(-1.0, -1.0, -1.0), Vector3D(1.0, 1.0, 1.0));
  Ray ray(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, 1.0));
  EXPECT_TRUE(box.hit(ray, 0.0, 100.0));
}

TEST(AABBTest, RayGrazingEdge) {
  AABB box(Vector3D(-1.0, -1.0, -1.0), Vector3D(1.0, 1.0, 1.0));
  Ray ray(Vector3D(1.0, 0.0, -5.0), Vector3D(0.0, 0.0, 1.0));
  EXPECT_FALSE(box.hit(ray, 0.0, 100.0));
}

TEST(AABBTest, RayParallelOutside) {
  AABB box(Vector3D(-1.0, -1.0, -1.0), Vector3D(1.0, 1.0, 1.0));
  Ray ray(Vector3D(2.0, 0.0, -5.0), Vector3D(0.0, 0.0, 1.0));
  EXPECT_FALSE(box.hit(ray, 0.0, 100.0));
}

TEST(AABBTest, RayBehindBox) {
  AABB box(Vector3D(-1.0, -1.0, -1.0), Vector3D(1.0, 1.0, 1.0));
  Ray ray(Vector3D(0.0, 0.0, -5.0), Vector3D(0.0, 0.0, -1.0));
  EXPECT_FALSE(box.hit(ray, 0.0, 100.0));
}

TEST(AABBTest, ContainsPointInside) {
  AABB box(Vector3D(-1.0, -1.0, -1.0), Vector3D(1.0, 1.0, 1.0));
  EXPECT_TRUE(box.contains(Vector3D(0.0, 0.0, 0.0)));
}

TEST(AABBTest, ContainsPointOutside) {
  AABB box(Vector3D(-1.0, -1.0, -1.0), Vector3D(1.0, 1.0, 1.0));
  EXPECT_FALSE(box.contains(Vector3D(2.0, 0.0, 0.0)));
}

TEST(AABBTest, ContainsPointOnBorder) {
  AABB box(Vector3D(-1.0, -1.0, -1.0), Vector3D(1.0, 1.0, 1.0));
  EXPECT_TRUE(box.contains(Vector3D(1.0, 0.0, 0.0)));
}

TEST(AABBTest, Surrounding) {
  AABB a(Vector3D(-1.0, -1.0, -1.0), Vector3D(0.0, 0.0, 0.0));
  AABB b(Vector3D(0.0, 0.0, 0.0), Vector3D(2.0, 2.0, 2.0));
  AABB s = AABB::surrounding(a, b);
  EXPECT_DOUBLE_EQ(s.min.x, -1.0);
  EXPECT_DOUBLE_EQ(s.min.y, -1.0);
  EXPECT_DOUBLE_EQ(s.min.z, -1.0);
  EXPECT_DOUBLE_EQ(s.max.x, 2.0);
  EXPECT_DOUBLE_EQ(s.max.y, 2.0);
  EXPECT_DOUBLE_EQ(s.max.z, 2.0);
}
