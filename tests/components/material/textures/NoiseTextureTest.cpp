/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** NoiseTexture unit tests
*/

#include <gtest/gtest.h>
#include <cmath>
#include "components/material/Textures/noise/NoiseTexture.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::materials::textures::NoiseTexture;
using raytracer::math::Color;
using raytracer::math::Vector3D;

TEST(NoiseTextureTest, SampleReturnsColorInValidRange) {
  NoiseTexture texture;

  Color result1 = texture.sample(0.0, 0.0, Vector3D(0.0, 0.0, 0.0));
  Color result2 = texture.sample(0.5, 0.5, Vector3D(1.5, 2.5, 3.5));
  Color result3 = texture.sample(1.0, 1.0, Vector3D(10.0, 20.0, 30.0));

  EXPECT_GE(result1.r, 0.0);
  EXPECT_LE(result1.r, 1.0);
  EXPECT_GE(result1.g, 0.0);
  EXPECT_LE(result1.g, 1.0);
  EXPECT_GE(result1.b, 0.0);
  EXPECT_LE(result1.b, 1.0);

  EXPECT_GE(result2.r, 0.0);
  EXPECT_LE(result2.r, 1.0);
  EXPECT_GE(result2.g, 0.0);
  EXPECT_LE(result2.g, 1.0);
  EXPECT_GE(result2.b, 0.0);
  EXPECT_LE(result2.b, 1.0);

  EXPECT_GE(result3.r, 0.0);
  EXPECT_LE(result3.r, 1.0);
  EXPECT_GE(result3.g, 0.0);
  EXPECT_LE(result3.g, 1.0);
  EXPECT_GE(result3.b, 0.0);
  EXPECT_LE(result3.b, 1.0);
}

TEST(NoiseTextureTest, SampleIgnoresUVCoordinates) {
  NoiseTexture texture;

  Color atOrigin = texture.sample(0.0, 0.0, Vector3D(5.0, 5.0, 5.0));
  Color atCorner = texture.sample(1.0, 1.0, Vector3D(5.0, 5.0, 5.0));

  EXPECT_DOUBLE_EQ(atOrigin.r, atCorner.r);
  EXPECT_DOUBLE_EQ(atOrigin.g, atCorner.g);
  EXPECT_DOUBLE_EQ(atOrigin.b, atCorner.b);
}

TEST(NoiseTextureTest, DifferentPointsProduceDifferentColors) {
  NoiseTexture texture;

  Color color1 = texture.sample(0.5, 0.5, Vector3D(0.0, 0.0, 0.0));
  Color color2 = texture.sample(0.5, 0.5, Vector3D(10.0, 10.0, 10.0));
  Color color3 = texture.sample(0.5, 0.5, Vector3D(-5.0, -5.0, -5.0));

  bool isDifferent = (color1.r != color2.r) || (color2.r != color3.r) ||
                     (color1.g != color2.g) || (color2.g != color3.g) ||
                     (color1.b != color2.b) || (color2.b != color3.b);
  EXPECT_TRUE(isDifferent);
}

TEST(NoiseTextureTest, NearbyPointsProduceSimilarColors) {
  NoiseTexture texture;

  Color color1 = texture.sample(0.5, 0.5, Vector3D(1.0, 1.0, 1.0));
  Color color2 = texture.sample(0.5, 0.5, Vector3D(1.05, 1.05, 1.05));

  double diffR = std::abs(color1.r - color2.r);
  double diffG = std::abs(color1.g - color2.g);
  double diffB = std::abs(color1.b - color2.b);

  EXPECT_LT(diffR, 0.2);
  EXPECT_LT(diffG, 0.2);
  EXPECT_LT(diffB, 0.2);
}

TEST(NoiseTextureTest, AllColorComponentsAreIdentical) {
  NoiseTexture texture;

  Color result = texture.sample(0.0, 0.0, Vector3D(5.0, 10.0, 15.0));

  EXPECT_DOUBLE_EQ(result.r, result.g);
  EXPECT_DOUBLE_EQ(result.g, result.b);
}

TEST(NoiseTextureTest, SampleWithNegativeCoordinates) {
  NoiseTexture texture;

  Color result = texture.sample(0.5, 0.5, Vector3D(-10.5, -20.3, -5.7));

  EXPECT_GE(result.r, 0.0);
  EXPECT_LE(result.r, 1.0);
  EXPECT_GE(result.g, 0.0);
  EXPECT_LE(result.g, 1.0);
  EXPECT_GE(result.b, 0.0);
  EXPECT_LE(result.b, 1.0);
}

TEST(NoiseTextureTest, SampleWithLargeCoordinates) {
  NoiseTexture texture;

  Color result = texture.sample(0.5, 0.5, Vector3D(1000.5, 2000.3, 3000.7));

  EXPECT_GE(result.r, 0.0);
  EXPECT_LE(result.r, 1.0);
  EXPECT_GE(result.g, 0.0);
  EXPECT_LE(result.g, 1.0);
  EXPECT_GE(result.b, 0.0);
  EXPECT_LE(result.b, 1.0);
}

TEST(NoiseTextureTest, SampleIsDeterministic) {
  NoiseTexture texture;

  Color result1 = texture.sample(0.5, 0.5, Vector3D(7.3, 11.2, 4.8));
  Color result2 = texture.sample(0.5, 0.5, Vector3D(7.3, 11.2, 4.8));

  EXPECT_DOUBLE_EQ(result1.r, result2.r);
  EXPECT_DOUBLE_EQ(result1.g, result2.g);
  EXPECT_DOUBLE_EQ(result1.b, result2.b);
}
