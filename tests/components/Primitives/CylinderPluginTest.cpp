/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Cylinder plugin integration tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "fixtures/CylinderPluginFixture.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::math::HitRecord;
using raytracer::math::Ray;
using raytracer::math::Vector3D;

namespace {

// Default cylinder: center=(0,0,0), axis=(0,1,0), radius=1, height=1
constexpr double defaultRadius = 1.0;
constexpr double defaultHalfHeight = 0.5;

}  // namespace

TEST_F(CylinderPluginFixture, ExposesCreateEntryPoint) {
  std::shared_ptr<IObject> obj = makeCylinder();
  ASSERT_NE(obj, nullptr);
}

TEST_F(CylinderPluginFixture, HitFront) {
  std::shared_ptr<IObject> cylinder = makeCylinder();
  // Ray from (2, 0, 0) toward (-1, 0, 0): hits the curved surface at x=1
  Ray ray(Vector3D(2.0, 0.0, 0.0), Vector3D(-1.0, 0.0, 0.0));
  HitRecord rec;
  EXPECT_TRUE(cylinder->hits(ray, 0.001, 1000.0, rec));
}

TEST_F(CylinderPluginFixture, LateralMiss) {
  std::shared_ptr<IObject> cylinder = makeCylinder();
  // Ray from (2, 0, 0) going in +Z: never crosses cylinder radius
  Ray ray(Vector3D(2.0, 0.0, 0.0), Vector3D(0.0, 0.0, 1.0));
  HitRecord rec;
  EXPECT_FALSE(cylinder->hits(ray, 0.001, 1000.0, rec));
}

TEST_F(CylinderPluginFixture, HeightMiss) {
  std::shared_ptr<IObject> cylinder = makeCylinder();
  // Ray from (2, 2, 0) toward (-1, 0, 0): hits cylinder wall at y=2, outside
  // [-0.5, 0.5]
  Ray ray(Vector3D(2.0, 2.0, 0.0), Vector3D(-1.0, 0.0, 0.0));
  HitRecord rec;
  EXPECT_FALSE(cylinder->hits(ray, 0.001, 1000.0, rec));
}

TEST_F(CylinderPluginFixture, FrontFaceNormal) {
  std::shared_ptr<IObject> cylinder = makeCylinder();
  Ray ray(Vector3D(2.0, 0.0, 0.0), Vector3D(-1.0, 0.0, 0.0));
  HitRecord rec;
  ASSERT_TRUE(cylinder->hits(ray, 0.001, 1000.0, rec));
  EXPECT_TRUE(rec.frontFace);
}

TEST_F(CylinderPluginFixture, BoundingBoxCoversAxis) {
  std::shared_ptr<IObject> cylinder = makeCylinder();
  const auto box = cylinder->getBoundingBox();
  EXPECT_NEAR(box.min.x, -defaultRadius, 1e-9);
  EXPECT_NEAR(box.min.y, -defaultHalfHeight, 1e-9);
  EXPECT_NEAR(box.min.z, -defaultRadius, 1e-9);
  EXPECT_NEAR(box.max.x, defaultRadius, 1e-9);
  EXPECT_NEAR(box.max.y, defaultHalfHeight, 1e-9);
  EXPECT_NEAR(box.max.z, defaultRadius, 1e-9);
}
