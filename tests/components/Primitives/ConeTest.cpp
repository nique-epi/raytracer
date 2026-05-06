/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ConeTest
*/

#include <gtest/gtest.h>
#include <memory>
#include <numbers>
#include "components/Primitives/cone/Cone.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using namespace raytracer::math;
using namespace raytracer::components::primitives;

TEST(ConeTest, HitFront) {
  Cone cone(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0),
            std::numbers::pi / 4.0, 2.0, nullptr);
  Ray ray(Vector3D(0.5, -1.0, 0.0), Vector3D(0.0, 1.0, 0.0));
  HitRecord rec;
  EXPECT_TRUE(cone.hits(ray, 0.001, 1000.0, rec));
  EXPECT_NEAR(rec.t, 1.5, 1e-9);
}

TEST(ConeTest, LateralMiss) {
  Cone cone(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0),
            std::numbers::pi / 4.0, 2.0, nullptr);
  Ray ray(Vector3D(5.0, 0.5, 0.0), Vector3D(0.0, 0.0, -1.0));
  HitRecord rec;
  EXPECT_FALSE(cone.hits(ray, 0.001, 1000.0, rec));
}

TEST(ConeTest, HeightMiss) {
  Cone cone(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0),
            std::numbers::pi / 4.0, 2.0, nullptr);
  Ray ray(Vector3D(3.0, -1.0, 0.0), Vector3D(0.0, 1.0, 0.0));
  HitRecord rec;
  EXPECT_FALSE(cone.hits(ray, 0.001, 1000.0, rec));
}

TEST(ConeTest, FrontFaceNormal) {
  Cone cone(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0),
            std::numbers::pi / 4.0, 2.0, nullptr);
  Ray ray(Vector3D(0.5, -1.0, 0.0), Vector3D(0.0, 1.0, 0.0));
  HitRecord rec;
  ASSERT_TRUE(cone.hits(ray, 0.001, 1000.0, rec));
  EXPECT_TRUE(rec.frontFace);
}

TEST(ConeTest, BoundingBoxAxisAligned) {
  Cone cone(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0),
            std::numbers::pi / 4.0, 2.0, nullptr);
  const AABB box = cone.getBoundingBox();
  EXPECT_NEAR(box.min.x, -2.0, 1e-9);
  EXPECT_NEAR(box.min.y, 0.0, 1e-9);
  EXPECT_NEAR(box.min.z, -2.0, 1e-9);
  EXPECT_NEAR(box.max.x, 2.0, 1e-9);
  EXPECT_NEAR(box.max.y, 2.0, 1e-9);
  EXPECT_NEAR(box.max.z, 2.0, 1e-9);
}
