/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Cone plugin integration tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "fixtures/ConePluginFixture.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::math::HitRecord;
using raytracer::math::Ray;
using raytracer::math::Vector3D;

namespace {

constexpr double defaultHeight = 1.0;
constexpr double defaultBaseRadius = defaultHeight * 1.0;  // tan(pi/4) == 1

}  // namespace

TEST_F(ConePluginFixture, ExposesCreateEntryPoint) {
  std::shared_ptr<IObject> obj = makeCone();
  ASSERT_NE(obj, nullptr);
}

TEST_F(ConePluginFixture, HitFront) {
  std::shared_ptr<IObject> cone = makeCone();
  const double x = defaultBaseRadius * 0.3;
  Ray ray(Vector3D(x, -1.0, 0.0), Vector3D(0.0, 1.0, 0.0));
  HitRecord rec;
  EXPECT_TRUE(cone->hits(ray, 0.001, 1000.0, rec));
}

TEST_F(ConePluginFixture, LateralMiss) {
  std::shared_ptr<IObject> cone = makeCone();
  const double x = defaultBaseRadius * 5.0;
  Ray ray(Vector3D(x, 0.5, 0.0), Vector3D(0.0, 0.0, -1.0));
  HitRecord rec;
  EXPECT_FALSE(cone->hits(ray, 0.001, 1000.0, rec));
}

TEST_F(ConePluginFixture, HeightMiss) {
  std::shared_ptr<IObject> cone = makeCone();
  const double x = defaultBaseRadius * 3.0;
  Ray ray(Vector3D(x, -1.0, 0.0), Vector3D(0.0, 1.0, 0.0));
  HitRecord rec;
  EXPECT_FALSE(cone->hits(ray, 0.001, 1000.0, rec));
}

TEST_F(ConePluginFixture, FrontFaceNormal) {
  std::shared_ptr<IObject> cone = makeCone();
  const double x = defaultBaseRadius * 0.3;
  Ray ray(Vector3D(x, -1.0, 0.0), Vector3D(0.0, 1.0, 0.0));
  HitRecord rec;
  ASSERT_TRUE(cone->hits(ray, 0.001, 1000.0, rec));
  EXPECT_TRUE(rec.frontFace);
}

TEST_F(ConePluginFixture, BoundingBoxCoversBase) {
  std::shared_ptr<IObject> cone = makeCone();
  const auto box = cone->getBoundingBox();
  EXPECT_NEAR(box.min.x, -defaultBaseRadius, 1e-9);
  EXPECT_NEAR(box.min.y, 0.0, 1e-9);
  EXPECT_NEAR(box.min.z, -defaultBaseRadius, 1e-9);
  EXPECT_NEAR(box.max.x, defaultBaseRadius, 1e-9);
  EXPECT_NEAR(box.max.y, defaultHeight, 1e-9);
  EXPECT_NEAR(box.max.z, defaultBaseRadius, 1e-9);
}
