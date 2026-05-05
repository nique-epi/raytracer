/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SphereTest
*/

#include <gtest/gtest.h>
#include <memory>
#include "components/Primitives/sphere/Sphere.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using namespace raytracer::math;
using namespace raytracer::components::primitives;

TEST(SphereTest, HitAtOrigin) {
  Sphere sphere(Vector3D(0.0, 0.0, -1.0), 0.5, nullptr);
  Ray ray(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, -1.0));
  HitRecord rec;
  EXPECT_TRUE(sphere.hits(ray, 0.001, 1000.0, rec));
  EXPECT_NEAR(rec.t, 0.5, 1e-9);
}

TEST(SphereTest, LateralMiss) {
  Sphere sphere(Vector3D(0.0, 5.0, -1.0), 0.5, nullptr);
  Ray ray(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, -1.0));
  HitRecord rec;
  EXPECT_FALSE(sphere.hits(ray, 0.001, 1000.0, rec));
}

TEST(SphereTest, HitTranslatedSphere) {
  Sphere sphere(Vector3D(0.0, 0.0, -5.0), 0.5, nullptr);
  Ray ray(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, -1.0));
  HitRecord rec;
  EXPECT_TRUE(sphere.hits(ray, 0.001, 1000.0, rec));
  EXPECT_NEAR(rec.t, 4.5, 1e-9);
}

TEST(SphereTest, OutwardNormal) {
  Sphere sphere(Vector3D(0.0, 0.0, -1.0), 0.5, nullptr);
  Ray ray(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, -1.0));
  HitRecord rec;
  ASSERT_TRUE(sphere.hits(ray, 0.001, 1000.0, rec));
  EXPECT_TRUE(rec.frontFace);
  EXPECT_NEAR(rec.normal.z, 1.0, 1e-9);
}

TEST(SphereTest, BoundingBoxCoversRadius) {
  Sphere sphere(Vector3D(1.0, 2.0, 3.0), 0.5, nullptr);
  const AABB box = sphere.getBoundingBox();
  EXPECT_NEAR(box.min.x, 0.5, 1e-9);
  EXPECT_NEAR(box.min.y, 1.5, 1e-9);
  EXPECT_NEAR(box.min.z, 2.5, 1e-9);
  EXPECT_NEAR(box.max.x, 1.5, 1e-9);
  EXPECT_NEAR(box.max.y, 2.5, 1e-9);
  EXPECT_NEAR(box.max.z, 3.5, 1e-9);
}
