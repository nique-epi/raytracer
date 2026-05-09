/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Cylinder plugin integration tests
*/

#include <gtest/gtest.h>
#include <limits>
#include "fixtures/CylinderPluginFixture.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::math::HitRecord;
using raytracer::math::Ray;
using raytracer::math::Vector3D;

namespace {

constexpr double radius = 1.0;
constexpr double height = 1.0;
constexpr double halfHeight = height / 2.0;

}  // namespace

TEST_F(CylinderPluginFixture, ExposesCreateAndDestroyEntryPoints) {
  IObject* obj =
      makeCylinder(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0), radius, height);
  ASSERT_NE(obj, nullptr);
}

TEST_F(CylinderPluginFixture, HitFront) {
  IObject* cylinder =
      makeCylinder(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0), radius, height);
  Ray ray(Vector3D(2.0, 0.0, 0.0), Vector3D(-1.0, 0.0, 0.0));
  HitRecord rec;
  EXPECT_TRUE(cylinder->hits(ray, 0.001, 1000.0, rec));
}

TEST_F(CylinderPluginFixture, LateralMiss) {
  IObject* cylinder =
      makeCylinder(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0), radius, height);
  Ray ray(Vector3D(2.0, 0.0, 0.0), Vector3D(0.0, 0.0, 1.0));
  HitRecord rec;
  EXPECT_FALSE(cylinder->hits(ray, 0.001, 1000.0, rec));
}

TEST_F(CylinderPluginFixture, HeightMiss) {
  IObject* cylinder =
      makeCylinder(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0), radius, height);
  Ray ray(Vector3D(2.0, 2.0, 0.0), Vector3D(-1.0, 0.0, 0.0));
  HitRecord rec;
  EXPECT_FALSE(cylinder->hits(ray, 0.001, 1000.0, rec));
}

TEST_F(CylinderPluginFixture, InfiniteCylinderHitsOutOfFiniteRange) {
  IObject* cylinder =
      makeCylinder(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0), radius, -1.0);
  Ray ray(Vector3D(2.0, 100.0, 0.0), Vector3D(-1.0, 0.0, 0.0));
  HitRecord rec;
  EXPECT_TRUE(cylinder->hits(ray, 0.001, 1000.0, rec));
}

TEST_F(CylinderPluginFixture, FrontFaceNormal) {
  IObject* cylinder =
      makeCylinder(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0), radius, height);
  Ray ray(Vector3D(2.0, 0.0, 0.0), Vector3D(-1.0, 0.0, 0.0));
  HitRecord rec;
  ASSERT_TRUE(cylinder->hits(ray, 0.001, 1000.0, rec));
  EXPECT_TRUE(rec.frontFace);
}

// Regression test: inf+1==inf (IEEE 754) — tMax=inf ne doit pas
// retourner une intersection fantôme quand le rayon rate le cylindre.
TEST_F(CylinderPluginFixture, HeightMissWithInfiniteTMax) {
  IObject* cylinder =
      makeCylinder(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0), radius, height);
  Ray ray(Vector3D(2.0, 2.0, 0.0), Vector3D(-1.0, 0.0, 0.0));
  HitRecord rec;
  const double inf = std::numeric_limits<double>::infinity();
  EXPECT_FALSE(cylinder->hits(ray, 0.001, inf, rec));
}

TEST_F(CylinderPluginFixture, BoundingBoxCoversAxis) {
  IObject* cylinder =
      makeCylinder(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0), radius, height);
  const auto box = cylinder->getBoundingBox();
  EXPECT_NEAR(box.min.x, -radius, 1e-9);
  EXPECT_NEAR(box.min.y, -halfHeight, 1e-9);
  EXPECT_NEAR(box.min.z, -radius, 1e-9);
  EXPECT_NEAR(box.max.x, radius, 1e-9);
  EXPECT_NEAR(box.max.y, halfHeight, 1e-9);
  EXPECT_NEAR(box.max.z, radius, 1e-9);
}
