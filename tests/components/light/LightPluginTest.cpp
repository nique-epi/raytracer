/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** LightPluginTest
*/

#include <gtest/gtest.h>
#include <memory>
#include "components/light/ILight.hpp"
#include "fixtures/AmbientFixture.hpp"
#include "fixtures/DirectionalFixture.hpp"
#include "fixtures/SphereFixture.hpp"
#include "scene/Scene.hpp"

namespace {

constexpr double epsilon = 1e-9;

}  // namespace

TEST_F(AmbientFixture, ExposesCreateAndDestroyEntryPoints) {
  ASSERT_NE(light, nullptr);
  ASSERT_NE(createFn, nullptr);
  ASSERT_NE(destroyFn, nullptr);
}

TEST_F(AmbientFixture, IlluminateReturnsColorTimesIntensity) {
  raytracer::scene::Scene scene;
  const raytracer::math::Vector3D point(0.0, 0.0, 0.0);

  const auto color = light->illuminate(point, scene);

  EXPECT_NEAR(color.r, 1.0, epsilon);
  EXPECT_NEAR(color.g, 1.0, epsilon);
  EXPECT_NEAR(color.b, 1.0, epsilon);
}

TEST_F(AmbientFixture, GetDirectionReturnsZeroVectorSentinel) {
  const raytracer::math::Vector3D point(1.0, 2.0, 3.0);

  const auto dir = light->getDirection(point);

  EXPECT_DOUBLE_EQ(dir.x, 0.0);
  EXPECT_DOUBLE_EQ(dir.y, 0.0);
  EXPECT_DOUBLE_EQ(dir.z, 0.0);
}

TEST_F(AmbientFixture, GetIntensityReturnsDefault) {
  EXPECT_DOUBLE_EQ(light->getIntensity(), 1.0);
}

TEST_F(AmbientFixture, IsOccludedAlwaysReturnsFalse) {
  raytracer::scene::Scene scene;
  const raytracer::math::Vector3D point(0.0, 0.0, 0.0);

  EXPECT_FALSE(light->isOccluded(point, scene));
}

// === Directional ===================================================

TEST_F(DirectionalLightFixture, ExposesCreateAndDestroyEntryPoints) {
  ASSERT_NE(light, nullptr);
  ASSERT_NE(createFn, nullptr);
  ASSERT_NE(destroyFn, nullptr);
}

TEST_F(DirectionalLightFixture, GetDirectionReturnsNormalizedDefault) {
  const raytracer::math::Vector3D point(0.0, 0.0, 0.0);

  const auto dir = light->getDirection(point);

  EXPECT_NEAR(dir.length(), 1.0, epsilon);
  EXPECT_NEAR(dir.x, 0.0, epsilon);
  EXPECT_NEAR(dir.y, -1.0, epsilon);
  EXPECT_NEAR(dir.z, 0.0, epsilon);
}

TEST_F(DirectionalLightFixture, GetIntensityReturnsDefault) {
  EXPECT_DOUBLE_EQ(light->getIntensity(), 1.0);
}

TEST_F(DirectionalLightFixture, IsOccludedReturnsFalseOnEmptyScene) {
  raytracer::scene::Scene scene;
  const raytracer::math::Vector3D point(0.0, 0.0, 0.0);

  EXPECT_FALSE(light->isOccluded(point, scene));
}

TEST_F(DirectionalLightFixture, IsOccludedReturnsTrueWhenSphereBlocksTheLight) {
  // Default directional light points at (0, -1, 0) → shadow ray from
  // the origin goes upward (0, +1, 0). A sphere placed above the
  // origin must intersect that shadow ray.
  raytracer::scene::Scene scene;
  scene.add(std::make_shared<SphereFixture>(
      raytracer::math::Vector3D(0.0, 5.0, 0.0), 1.0));
  const raytracer::math::Vector3D point(0.0, 0.0, 0.0);

  EXPECT_TRUE(light->isOccluded(point, scene));
}

TEST_F(DirectionalLightFixture,
       IlluminateReturnsColorTimesIntensityWhenNotOccluded) {
  raytracer::scene::Scene scene;
  const raytracer::math::Vector3D point(0.0, 0.0, 0.0);

  const auto color = light->illuminate(point, scene);

  EXPECT_NEAR(color.r, 1.0, epsilon);
  EXPECT_NEAR(color.g, 1.0, epsilon);
  EXPECT_NEAR(color.b, 1.0, epsilon);
}

TEST_F(DirectionalLightFixture, IlluminateReturnsBlackWhenOccluded) {
  raytracer::scene::Scene scene;
  scene.add(std::make_shared<SphereFixture>(
      raytracer::math::Vector3D(0.0, 5.0, 0.0), 1.0));
  const raytracer::math::Vector3D point(0.0, 0.0, 0.0);

  const auto color = light->illuminate(point, scene);

  EXPECT_DOUBLE_EQ(color.r, 0.0);
  EXPECT_DOUBLE_EQ(color.g, 0.0);
  EXPECT_DOUBLE_EQ(color.b, 0.0);
}
