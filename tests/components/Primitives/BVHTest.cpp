/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** BVHNode unit tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../fixtures/SphereFixture.hpp"
#include "components/Primitives/BVH/BVHNode.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using namespace raytracer::math;
using raytracer::components::BVHNode;

TEST(BVHTest, SingleObjectHit) {
  std::vector<std::shared_ptr<IObject>> objects;
  objects.push_back(std::make_shared<SphereFixture>(Vector3D(0, 0, -2), 0.5));

  BVHNode bvh(std::move(objects));

  Ray ray(Vector3D(0, 0, 0), Vector3D(0, 0, -1));
  HitRecord rec;
  EXPECT_TRUE(bvh.hits(ray, 0.001, 1000.0, rec));
  EXPECT_NEAR(rec.t, 1.5, 1e-6);
}

TEST(BVHTest, SingleObjectMiss) {
  std::vector<std::shared_ptr<IObject>> objects;
  objects.push_back(std::make_shared<SphereFixture>(Vector3D(5, 0, -2), 0.5));

  BVHNode bvh(std::move(objects));

  Ray ray(Vector3D(0, 0, 0), Vector3D(0, 0, -1));
  HitRecord rec;
  EXPECT_FALSE(bvh.hits(ray, 0.001, 1000.0, rec));
}

TEST(BVHTest, TwoObjects_PicksNearest) {
  std::vector<std::shared_ptr<IObject>> objects;
  objects.push_back(std::make_shared<SphereFixture>(Vector3D(0, 0, -3), 0.5));
  objects.push_back(std::make_shared<SphereFixture>(Vector3D(0, 0, -1), 0.5));

  BVHNode bvh(std::move(objects));

  Ray ray(Vector3D(0, 0, 0), Vector3D(0, 0, -1));
  HitRecord rec;
  EXPECT_TRUE(bvh.hits(ray, 0.001, 1000.0, rec));
  EXPECT_NEAR(rec.t, 0.5, 1e-6);
}

TEST(BVHTest, ManyObjects_PicksNearest) {
  std::vector<std::shared_ptr<IObject>> objects;
  objects.push_back(std::make_shared<SphereFixture>(Vector3D(0, 0, -5), 0.4));
  objects.push_back(std::make_shared<SphereFixture>(Vector3D(0, 0, -4), 0.4));
  objects.push_back(std::make_shared<SphereFixture>(Vector3D(0, 0, -3), 0.4));
  objects.push_back(std::make_shared<SphereFixture>(Vector3D(0, 0, -2), 0.4));
  objects.push_back(std::make_shared<SphereFixture>(Vector3D(0, 0, -1), 0.4));

  BVHNode bvh(std::move(objects));

  Ray ray(Vector3D(0, 0, 0), Vector3D(0, 0, -1));
  HitRecord rec;
  EXPECT_TRUE(bvh.hits(ray, 0.001, 1000.0, rec));
  EXPECT_NEAR(rec.t, 0.6, 1e-6);
}

TEST(BVHTest, ManyObjects_Miss) {
  std::vector<std::shared_ptr<IObject>> objects;
  objects.push_back(std::make_shared<SphereFixture>(Vector3D(3, 0, -1), 0.4));
  objects.push_back(std::make_shared<SphereFixture>(Vector3D(3, 0, -2), 0.4));
  objects.push_back(std::make_shared<SphereFixture>(Vector3D(3, 0, -3), 0.4));

  BVHNode bvh(std::move(objects));

  Ray ray(Vector3D(0, 0, 0), Vector3D(0, 0, -1));
  HitRecord rec;
  EXPECT_FALSE(bvh.hits(ray, 0.001, 1000.0, rec));
}

TEST(BVHTest, BoundingBoxEncompassesAll) {
  std::vector<std::shared_ptr<IObject>> objects;
  objects.push_back(std::make_shared<SphereFixture>(Vector3D(-3, 0, 0), 0.5));
  objects.push_back(std::make_shared<SphereFixture>(Vector3D(3, 0, 0), 0.5));

  BVHNode bvh(std::move(objects));
  const AABB box = bvh.getBoundingBox();

  EXPECT_LE(box.min.x, -3.0);
  EXPECT_GE(box.max.x, 3.0);
}

TEST(BVHTest, ObjectsAlongX_PicksCorrectHit) {
  std::vector<std::shared_ptr<IObject>> objects;
  for (int xOffset = -2; xOffset <= 2; ++xOffset) {
    objects.push_back(std::make_shared<SphereFixture>(
        Vector3D(static_cast<double>(xOffset) * 3.0, 0, -5), 0.5));
  }

  BVHNode bvh(std::move(objects));

  Ray ray(Vector3D(0, 0, 0), Vector3D(0, 0, -1));
  HitRecord rec;
  EXPECT_TRUE(bvh.hits(ray, 0.001, 1000.0, rec));
  EXPECT_NEAR(rec.t, 4.5, 1e-6);
}
