/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** CheckerTexture unit tests
*/

#include <gtest/gtest.h>
#include <cmath>
#include "components/material/Textures/checker/CheckerTexture.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::materials::textures::CheckerTexture;
using raytracer::math::Color;
using raytracer::math::Vector3D;

namespace {

constexpr double scale = 10.0;

bool sinProductIsNegative(double s, const Vector3D& point) {
  return std::sin(s * point.x) * std::sin(s * point.y) * std::sin(s * point.z) <
         0.0;
}

}  // namespace

TEST(CheckerTextureTest, ReturnsOddWhenSineProductNegative) {
  Color odd(1.0, 0.0, 0.0);
  Color even(0.0, 0.0, 1.0);
  CheckerTexture texture(odd, even, scale);

  Vector3D point(0.4, 0.1, 0.1);
  ASSERT_TRUE(sinProductIsNegative(scale, point));

  Color result = texture.sample(0.0, 0.0, point);
  EXPECT_DOUBLE_EQ(result.r, odd.r);
  EXPECT_DOUBLE_EQ(result.g, odd.g);
  EXPECT_DOUBLE_EQ(result.b, odd.b);
}

TEST(CheckerTextureTest, ReturnsEvenWhenSineProductNonNegative) {
  Color odd(1.0, 0.0, 0.0);
  Color even(0.0, 0.0, 1.0);
  CheckerTexture texture(odd, even, scale);

  // sin(1.0)^3 > 0 → product positive
  Vector3D point(0.1, 0.1, 0.1);
  ASSERT_FALSE(sinProductIsNegative(scale, point));

  Color result = texture.sample(0.0, 0.0, point);
  EXPECT_DOUBLE_EQ(result.r, even.r);
  EXPECT_DOUBLE_EQ(result.g, even.g);
  EXPECT_DOUBLE_EQ(result.b, even.b);
}

TEST(CheckerTextureTest, AlternatesBetweenCells) {
  Color odd(1.0, 0.0, 0.0);
  Color even(0.0, 0.0, 1.0);
  CheckerTexture texture(odd, even, scale);

  Vector3D pointOdd(0.4, 0.1, 0.1);
  Vector3D pointEven(0.1, 0.1, 0.1);

  Color resultOdd = texture.sample(0.0, 0.0, pointOdd);
  Color resultEven = texture.sample(0.0, 0.0, pointEven);

  EXPECT_NE(resultOdd.r, resultEven.r);
}

TEST(CheckerTextureTest, SampleIgnoresUVCoordinates) {
  Color odd(1.0, 0.0, 0.0);
  Color even(0.0, 0.0, 1.0);
  CheckerTexture texture(odd, even, scale);

  Vector3D point(0.4, 0.1, 0.1);

  Color atUV00 = texture.sample(0.0, 0.0, point);
  Color atUV11 = texture.sample(1.0, 1.0, point);

  EXPECT_DOUBLE_EQ(atUV00.r, atUV11.r);
  EXPECT_DOUBLE_EQ(atUV00.g, atUV11.g);
  EXPECT_DOUBLE_EQ(atUV00.b, atUV11.b);
}
