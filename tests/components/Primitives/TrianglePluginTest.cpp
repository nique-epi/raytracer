/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Triangle plugin integration tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "fixtures/TrianglePluginFixture.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::math::HitRecord;
using raytracer::math::Ray;
using raytracer::math::Vector3D;

TEST_F(TrianglePluginFixture, ExposesCreateEntryPoint) {
  std::shared_ptr<IObject> obj = makeTriangle();
  ASSERT_NE(obj, nullptr);
}

TEST_F(TrianglePluginFixture, HitFront) {
  std::shared_ptr<IObject> triangle = makeTriangle();
  Ray ray(Vector3D(0.25, 0.25, 1.0), Vector3D(0.0, 0.0, -1.0));
  HitRecord rec;
  ASSERT_TRUE(triangle->hits(ray, 0.001, 1000.0, rec));
  EXPECT_NEAR(rec.t, 1.0, 1e-9);
  EXPECT_NEAR(rec.point.x, 0.25, 1e-9);
  EXPECT_NEAR(rec.point.y, 0.25, 1e-9);
  EXPECT_NEAR(rec.point.z, 0.0, 1e-9);
  EXPECT_TRUE(rec.frontFace);
}

TEST_F(TrianglePluginFixture, MissOutsideBarycentricDomain) {
  std::shared_ptr<IObject> triangle = makeTriangle();
  Ray ray(Vector3D(1.2, 1.2, 1.0), Vector3D(0.0, 0.0, -1.0));
  HitRecord rec;
  EXPECT_FALSE(triangle->hits(ray, 0.001, 1000.0, rec));
}

TEST_F(TrianglePluginFixture, BoundingBoxCoversVertices) {
  std::shared_ptr<IObject> triangle = makeTriangle();
  const auto box = triangle->getBoundingBox();
  EXPECT_NEAR(box.min.x, 0.0, 1e-9);
  EXPECT_NEAR(box.min.y, 0.0, 1e-9);
  EXPECT_NEAR(box.min.z, 0.0, 1e-9);
  EXPECT_NEAR(box.max.x, 1.0, 1e-9);
  EXPECT_NEAR(box.max.y, 1.0, 1e-9);
  EXPECT_NEAR(box.max.z, 0.0, 1e-9);
}
