/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneTest
*/

#include <gtest/gtest.h>
#include <memory>
#include "../fixtures/OrthoCameraFixture.hpp"
#include "../fixtures/SphereFixture.hpp"
#include "fixtures/NullLightFixture.hpp"
#include "scene/Scene.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using namespace raytracer::math;
using namespace raytracer::scene;

TEST(SceneTest, HitReturnsFalseWhenEmpty) {
  Scene scene;
  Ray r(Vector3D(0, 0, 0), Vector3D(0, 0, -1));
  HitRecord rec;
  EXPECT_FALSE(scene.hit(r, 0.001, 1000.0, rec));
}

TEST(SceneTest, HitReturnsTrueAfterAddingSphere) {
  Scene scene;
  scene.add(std::make_shared<SphereFixture>(Vector3D(0, 0, -1), 0.5));
  Ray r(Vector3D(0, 0, 0), Vector3D(0, 0, -1));
  HitRecord rec;
  EXPECT_TRUE(scene.hit(r, 0.001, 1000.0, rec));
}

TEST(SceneTest, HitPicksNearestObject) {
  Scene scene;
  scene.add(std::make_shared<SphereFixture>(Vector3D(0, 0, -3), 0.5));
  scene.add(std::make_shared<SphereFixture>(Vector3D(0, 0, -1), 0.5));
  Ray r(Vector3D(0, 0, 0), Vector3D(0, 0, -1));
  HitRecord rec;
  ASSERT_TRUE(scene.hit(r, 0.001, 1000.0, rec));
  EXPECT_NEAR(rec.t, 0.5, 1e-6);
}

TEST(SceneTest, GetCameraReturnsNullptrByDefault) {
  Scene scene;
  EXPECT_EQ(scene.getCamera(), nullptr);
}

TEST(SceneTest, SetAndGetCamera) {
  Scene scene;
  auto cam = std::make_shared<OrthoCameraFixture>();
  scene.setCamera(cam);
  EXPECT_EQ(scene.getCamera(), cam);
}

TEST(SceneTest, GetLightsEmptyByDefault) {
  Scene scene;
  EXPECT_TRUE(scene.getLights().empty());
}

TEST(SceneTest, AddLightIncreasesCount) {
  Scene scene;
  scene.addLight(std::make_shared<NullLightFixture>());
  scene.addLight(std::make_shared<NullLightFixture>());
  EXPECT_EQ(scene.getLights().size(), 2u);
}

TEST(SceneTest, HitMissesOutsideTRange) {
  Scene scene;
  scene.add(std::make_shared<SphereFixture>(Vector3D(0, 0, -1), 0.5));
  Ray r(Vector3D(0, 0, 0), Vector3D(0, 0, -1));
  HitRecord rec;
  EXPECT_FALSE(scene.hit(r, 2.0, 1000.0, rec));
}
