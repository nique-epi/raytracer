/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PerspectiveCameraPluginTest
*/

#include <gtest/gtest.h>
#include "../../fixture/components/camera/perspective/PerspectiveCameraFixture.hpp"

TEST_F(PerspectiveCameraFixture, ExposesCreateAndDestroyEntryPoints) {
  ICamera* anotherCamera = createCameraFn();
  ASSERT_NE(anotherCamera, nullptr);

  destroyCameraFn(anotherCamera);
}

TEST_F(PerspectiveCameraFixture, CenterRayPointsForward) {
  ASSERT_NE(camera, nullptr);

  camera->setResolution(1920, 1080);
  const raytracer::math::Ray ray = camera->getRay(0.5, 0.5);

  EXPECT_DOUBLE_EQ(ray.getOrigin().x, 0.0);
  EXPECT_DOUBLE_EQ(ray.getOrigin().y, 0.0);
  EXPECT_DOUBLE_EQ(ray.getOrigin().z, 0.0);
  EXPECT_DOUBLE_EQ(ray.getDirection().x, 0.0);
  EXPECT_DOUBLE_EQ(ray.getDirection().y, 0.0);
  EXPECT_DOUBLE_EQ(ray.getDirection().z, -1.0);
}
