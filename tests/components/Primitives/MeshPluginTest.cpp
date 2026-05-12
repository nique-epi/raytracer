/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Mesh plugin integration tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "fixtures/MeshPluginFixture.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::math::HitRecord;
using raytracer::math::Ray;
using raytracer::math::Vector3D;

TEST_F(MeshPluginFixture, ExposesCreateEntryPoint) {
  std::shared_ptr<IObject> obj = makeMesh();
  ASSERT_NE(obj, nullptr);
}

TEST_F(MeshPluginFixture, EmptyMeshDoesNotHit) {
  std::shared_ptr<IObject> mesh = makeMesh();
  Ray ray(Vector3D(0.25, 0.25, 1.0), Vector3D(0.0, 0.0, -1.0));
  HitRecord rec;
  EXPECT_FALSE(mesh->hits(ray, 0.001, 1000.0, rec));
}

TEST_F(MeshPluginFixture, MissOutsideMesh) {
  std::shared_ptr<IObject> mesh = makeMesh();
  Ray ray(Vector3D(2.0, 2.0, 1.0), Vector3D(0.0, 0.0, -1.0));
  HitRecord rec;
  EXPECT_FALSE(mesh->hits(ray, 0.001, 1000.0, rec));
}

TEST_F(MeshPluginFixture, BoundingBoxIsZeroWhenEmpty) {
  std::shared_ptr<IObject> mesh = makeMesh();
  const auto box = mesh->getBoundingBox();
  EXPECT_NEAR(box.min.x, 0.0, 1e-9);
  EXPECT_NEAR(box.min.y, 0.0, 1e-9);
  EXPECT_NEAR(box.min.z, 0.0, 1e-9);
  EXPECT_NEAR(box.max.x, 0.0, 1e-9);
  EXPECT_NEAR(box.max.y, 0.0, 1e-9);
  EXPECT_NEAR(box.max.z, 0.0, 1e-9);
}
