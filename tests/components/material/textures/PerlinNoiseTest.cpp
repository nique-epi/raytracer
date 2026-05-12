/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PerlinNoise unit tests
*/

#include <gtest/gtest.h>
#include <cmath>
#include "components/material/Textures/noise/perlinNoise/PerlinNoise.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::materials::textures::utils::PerlinNoise;
using raytracer::math::Vector3D;

class PerlinNoiseTest : public ::testing::Test {
 protected:
  PerlinNoise perlin;
};

TEST_F(PerlinNoiseTest, NoiseReturnsValueInValidRange) {
  Vector3D point1(0.5, 0.5, 0.5);
  Vector3D point2(1.234, 5.678, 9.012);
  Vector3D point3(-3.14, 2.71, 1.41);

  double noise1 = perlin.noise(point1);
  double noise2 = perlin.noise(point2);
  double noise3 = perlin.noise(point3);

  EXPECT_GE(noise1, -1.0);
  EXPECT_LE(noise1, 1.0);
  EXPECT_GE(noise2, -1.0);
  EXPECT_LE(noise2, 1.0);
  EXPECT_GE(noise3, -1.0);
  EXPECT_LE(noise3, 1.0);
}

TEST_F(PerlinNoiseTest, NearbyPointsProduceSimilarValues) {
  Vector3D point(1.0, 2.0, 3.0);
  Vector3D nearby(1.01, 2.01, 3.01);

  double noise1 = perlin.noise(point);
  double noise2 = perlin.noise(nearby);

  double difference = std::abs(noise1 - noise2);
  EXPECT_LT(difference, 0.2);
}

TEST_F(PerlinNoiseTest, VeryDifferentPointsProduceDifferentValues) {
  Vector3D point1(0.1, 0.2, 0.3);
  Vector3D point2(50.1, 60.2, 70.3);

  double noise1 = perlin.noise(point1);
  double noise2 = perlin.noise(point2);

  EXPECT_NE(noise1, noise2);
}

TEST_F(PerlinNoiseTest, TurbulenceDepth0Returns0) {
  Vector3D point(1.5, 2.5, 3.5);

  double turb = perlin.turbulence(point, 0);

  EXPECT_DOUBLE_EQ(turb, 0.0);
}

TEST_F(PerlinNoiseTest, TurbulenceReturnsNonNegative) {
  Vector3D point1(0.5, 0.5, 0.5);
  Vector3D point2(1.234, 5.678, 9.012);
  Vector3D point3(-3.14, 2.71, 1.41);

  double turb1 = perlin.turbulence(point1, 3);
  double turb2 = perlin.turbulence(point2, 5);
  double turb3 = perlin.turbulence(point3, 7);

  EXPECT_GE(turb1, 0.0);
  EXPECT_GE(turb2, 0.0);
  EXPECT_GE(turb3, 0.0);
}

TEST_F(PerlinNoiseTest, TurbulenceIncreaseWithDepth) {
  Vector3D point(1.5, 2.5, 3.5);

  double turb1 = perlin.turbulence(point, 1);
  double turb2 = perlin.turbulence(point, 3);
  double turb3 = perlin.turbulence(point, 7);

  EXPECT_LE(turb1, turb2);
  EXPECT_LE(turb2, turb3);
}

TEST_F(PerlinNoiseTest, TurbulenceIsDeterministic) {
  Vector3D point(3.14, 2.71, 1.41);

  double turb1 = perlin.turbulence(point, 5);
  double turb2 = perlin.turbulence(point, 5);

  EXPECT_DOUBLE_EQ(turb1, turb2);
}

TEST_F(PerlinNoiseTest, NoiseAtIntegerCoordinates) {
  Vector3D origin(0.0, 0.0, 0.0);
  Vector3D intPoint(5.0, 10.0, 15.0);
  Vector3D negIntPoint(-3.0, -7.0, -2.0);

  double noise1 = perlin.noise(origin);
  double noise2 = perlin.noise(intPoint);
  double noise3 = perlin.noise(negIntPoint);

  EXPECT_GE(noise1, -1.0);
  EXPECT_LE(noise1, 1.0);
  EXPECT_GE(noise2, -1.0);
  EXPECT_LE(noise2, 1.0);
  EXPECT_GE(noise3, -1.0);
  EXPECT_LE(noise3, 1.0);
}

TEST_F(PerlinNoiseTest, NoiseWithNegativeCoordinates) {
  Vector3D point(-10.5, -20.3, -5.7);

  double noise = perlin.noise(point);

  EXPECT_GE(noise, -1.0);
  EXPECT_LE(noise, 1.0);
}

TEST_F(PerlinNoiseTest, NoiseWithLargeCoordinates) {
  Vector3D point(1000.5, 2000.3, 3000.7);

  double noise = perlin.noise(point);

  EXPECT_GE(noise, -1.0);
  EXPECT_LE(noise, 1.0);
}
