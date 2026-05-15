/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PointLight unit tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "components/light/point/Point.hpp"
#include "fixtures/SphereFixture.hpp"
#include "scene/Scene.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Constants.hpp"
#include "utils/math/Vector3D.hpp"

namespace {

using raytracer::components::light::point::PointLight;
using raytracer::math::Color;
using raytracer::math::Vector3D;
using raytracer::scene::Scene;

TEST(PointLightTest, GetIntensityReturnsConfiguredValue) {
  PointLight light(Vector3D(0.0, 1.0, 0.0), Color(1.0, 1.0, 1.0), 0.7);
  EXPECT_DOUBLE_EQ(light.getIntensity(), 0.7);
}

TEST(PointLightTest, GetDirectionPointsFromSourceToSurface) {
  PointLight light(Vector3D(0.0, 0.0, 0.0), Color(1.0, 1.0, 1.0), 1.0);
  const Vector3D dir = light.getDirection(Vector3D(2.0, 0.0, 0.0));
  EXPECT_DOUBLE_EQ(dir.x, 1.0);
  EXPECT_DOUBLE_EQ(dir.y, 0.0);
  EXPECT_DOUBLE_EQ(dir.z, 0.0);
}

TEST(PointLightTest, IlluminateAppliesInverseSquareFalloff) {
  Scene scene;
  PointLight light(Vector3D(0.0, 0.0, 0.0), Color(1.0, 1.0, 1.0), 4.0);
  const Color nearColor = light.illuminate(Vector3D(2.0, 0.0, 0.0), scene);
  const Color farColor = light.illuminate(Vector3D(4.0, 0.0, 0.0), scene);
  EXPECT_NEAR(nearColor.r, 1.0 / (4.0 * raytracer::math::constants::PI), 1e-9);
  EXPECT_NEAR(farColor.r, 1.0 / (16.0 * raytracer::math::constants::PI), 1e-9);
}

TEST(PointLightTest, IsOccludedDetectsBlockerBetweenPointAndLight) {
  Scene scene;
  scene.add(std::make_shared<SphereFixture>(Vector3D(1.0, 0.0, 0.0), 0.5));
  PointLight light(Vector3D(2.0, 0.0, 0.0), Color(1.0, 1.0, 1.0), 1.0);
  EXPECT_TRUE(light.isOccluded(Vector3D(0.0, 0.0, 0.0), scene));
}

TEST(PointLightTest, IsOccludedIgnoresBlockerBehindTheLight) {
  Scene scene;
  scene.add(std::make_shared<SphereFixture>(Vector3D(4.0, 0.0, 0.0), 0.5));
  PointLight light(Vector3D(2.0, 0.0, 0.0), Color(1.0, 1.0, 1.0), 1.0);
  EXPECT_FALSE(light.isOccluded(Vector3D(0.0, 0.0, 0.0), scene));
}

TEST(PointLightTest, IlluminateReturnsBlackWhenOccluded) {
  Scene scene;
  scene.add(std::make_shared<SphereFixture>(Vector3D(1.0, 0.0, 0.0), 0.5));
  PointLight light(Vector3D(2.0, 0.0, 0.0), Color(1.0, 1.0, 1.0), 4.0);
  const Color c = light.illuminate(Vector3D(0.0, 0.0, 0.0), scene);
  EXPECT_DOUBLE_EQ(c.r, 0.0);
  EXPECT_DOUBLE_EQ(c.g, 0.0);
  EXPECT_DOUBLE_EQ(c.b, 0.0);
}

// Degenerate case: a shading point landing exactly on a PointLight's
// position would historically crash the renderer because
// Vector3D::normalize() throws on zero-length input. The implementation
// must return the zero-vector sentinel (the same one used by Ambient)
// without throwing.
TEST(PointLightTest, GetDirectionAtLightPositionReturnsZeroVector) {
  const Vector3D lightPos(1.5, -2.0, 0.5);
  PointLight light(lightPos, Color(1.0, 1.0, 1.0), 1.0);
  const Vector3D dir = light.getDirection(lightPos);
  EXPECT_DOUBLE_EQ(dir.x, 0.0);
  EXPECT_DOUBLE_EQ(dir.y, 0.0);
  EXPECT_DOUBLE_EQ(dir.z, 0.0);
}

// Degenerate case: isOccluded() previously divided distance by itself
// to build the shadow-ray direction, producing NaN when distance is
// zero. The implementation must short-circuit to "not occluded" so a
// malformed scene cannot poison the renderer with NaN colours.
TEST(PointLightTest, IsOccludedAtLightPositionReturnsFalse) {
  const Vector3D lightPos(0.0, 0.0, 0.0);
  Scene scene;
  scene.add(std::make_shared<SphereFixture>(Vector3D(1.0, 0.0, 0.0), 0.5));
  PointLight light(lightPos, Color(1.0, 1.0, 1.0), 1.0);
  EXPECT_FALSE(light.isOccluded(lightPos, scene));
}

// Degenerate case: physically the radiance at the light's exact
// position diverges (1 / 0 falloff). The implementation must skip the
// contribution and return black rather than emit inf or NaN.
TEST(PointLightTest, IlluminateAtLightPositionReturnsBlack) {
  const Vector3D lightPos(0.0, 0.0, 0.0);
  Scene scene;
  PointLight light(lightPos, Color(1.0, 1.0, 1.0), 1.0);
  const Color c = light.illuminate(lightPos, scene);
  EXPECT_DOUBLE_EQ(c.r, 0.0);
  EXPECT_DOUBLE_EQ(c.g, 0.0);
  EXPECT_DOUBLE_EQ(c.b, 0.0);
}

}  // namespace
