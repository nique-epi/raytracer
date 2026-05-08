/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Plane plugin integration tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "fixtures/PlanePluginFixture.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::math::HitRecord;
using raytracer::math::Ray;
using raytracer::math::Vector3D;

TEST_F(PlanePluginFixture, ExposesCreateEntryPoint) {
  std::shared_ptr<IObject> obj = makePlane();
  ASSERT_NE(obj, nullptr);
}

TEST_F(PlanePluginFixture, HitFront) {
  std::shared_ptr<IObject> plane = makePlane();
  Ray ray(Vector3D(0.0, 5.0, 0.0), Vector3D(0.0, -1.0, 0.0));
  HitRecord rec;
  ASSERT_TRUE(plane->hits(ray, 0.001, 1000.0, rec));
  EXPECT_NEAR(rec.t, 5.0, 1e-9);
  EXPECT_TRUE(rec.frontFace);
}

TEST_F(PlanePluginFixture, HitBackFace) {
  std::shared_ptr<IObject> plane = makePlane();
  Ray ray(Vector3D(0.0, -5.0, 0.0), Vector3D(0.0, 1.0, 0.0));
  HitRecord rec;
  ASSERT_TRUE(plane->hits(ray, 0.001, 1000.0, rec));
  EXPECT_NEAR(rec.t, 5.0, 1e-9);
  EXPECT_FALSE(rec.frontFace);
}

TEST_F(PlanePluginFixture, ParallelMiss) {
  std::shared_ptr<IObject> plane = makePlane();
  Ray ray(Vector3D(0.0, 1.0, 0.0), Vector3D(1.0, 0.0, 0.0));
  HitRecord rec;
  EXPECT_FALSE(plane->hits(ray, 0.001, 1000.0, rec));
}
