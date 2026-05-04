/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Sphere plugin integration tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "fixture/SpherePluginFixture.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::math::HitRecord;
using raytracer::math::Ray;
using raytracer::math::Vector3D;

TEST_F(SpherePluginFixture, ExposesCreateAndDestroyEntryPoints) {
  std::shared_ptr<IObject> obj = makeSphere();
  ASSERT_NE(obj, nullptr);
}

TEST_F(SpherePluginFixture, HitAtOrigin) {
  std::shared_ptr<IObject> sphere = makeSphere();
  Ray ray(Vector3D(0.0, 0.0, 3.0), Vector3D(0.0, 0.0, -1.0));
  HitRecord rec;
  EXPECT_TRUE(sphere->hits(ray, 0.001, 1000.0, rec));
}

TEST_F(SpherePluginFixture, LateralMiss) {
  std::shared_ptr<IObject> sphere = makeSphere();
  Ray ray(Vector3D(0.0, 5.0, 0.0), Vector3D(0.0, 0.0, -1.0));
  HitRecord rec;
  EXPECT_FALSE(sphere->hits(ray, 0.001, 1000.0, rec));
}

TEST_F(SpherePluginFixture, BoundingBoxCoversRadius) {
  std::shared_ptr<IObject> sphere = makeSphere();
  const auto box = sphere->getBoundingBox();
  EXPECT_NEAR(box.min.x, -1.0, 1e-9);
  EXPECT_NEAR(box.min.y, -1.0, 1e-9);
  EXPECT_NEAR(box.min.z, -1.0, 1e-9);
  EXPECT_NEAR(box.max.x, 1.0, 1e-9);
  EXPECT_NEAR(box.max.y, 1.0, 1e-9);
  EXPECT_NEAR(box.max.z, 1.0, 1e-9);
}

TEST_F(SpherePluginFixture, OutwardNormal) {
  std::shared_ptr<IObject> sphere = makeSphere();
  Ray ray(Vector3D(0.0, 0.0, 3.0), Vector3D(0.0, 0.0, -1.0));
  HitRecord rec;
  ASSERT_TRUE(sphere->hits(ray, 0.001, 1000.0, rec));
  EXPECT_TRUE(rec.frontFace);
}
