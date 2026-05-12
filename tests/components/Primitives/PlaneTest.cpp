/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PlaneTest
*/

#include <gtest/gtest.h>
#include <memory>
#include "components/Primitives/plane/Plane.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using namespace raytracer::math;
using namespace raytracer::components::primitives;

TEST(PlaneTest, HitFront) {
  Plane plane(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0), nullptr);
  Ray ray(Vector3D(0.0, 5.0, 0.0), Vector3D(0.0, -1.0, 0.0));
  HitRecord rec;
  ASSERT_TRUE(plane.hits(ray, 0.001, 1000.0, rec));
  EXPECT_NEAR(rec.t, 5.0, 1e-9);
  EXPECT_TRUE(rec.frontFace);
}

TEST(PlaneTest, HitBackFace) {
  Plane plane(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0), nullptr);
  Ray ray(Vector3D(0.0, -5.0, 0.0), Vector3D(0.0, 1.0, 0.0));
  HitRecord rec;
  ASSERT_TRUE(plane.hits(ray, 0.001, 1000.0, rec));
  EXPECT_NEAR(rec.t, 5.0, 1e-9);
  EXPECT_FALSE(rec.frontFace);
}

TEST(PlaneTest, ParallelMiss) {
  Plane plane(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0), nullptr);
  Ray ray(Vector3D(0.0, 1.0, 0.0), Vector3D(1.0, 0.0, 0.0));
  HitRecord rec;
  EXPECT_FALSE(plane.hits(ray, 0.001, 1000.0, rec));
}
