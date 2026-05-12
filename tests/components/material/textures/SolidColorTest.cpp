/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SolidColor unit tests
*/

#include <gtest/gtest.h>
#include "components/material/Textures/solid/SolidColor.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::materials::textures::SolidColor;
using raytracer::math::Color;
using raytracer::math::Vector3D;

TEST(SolidColorTest, SampleReturnsConstructorColor) {
  Color red(1.0, 0.0, 0.0);
  SolidColor texture(red);

  Color result = texture.sample(0.0, 0.0, Vector3D(0.0, 0.0, 0.0));

  EXPECT_DOUBLE_EQ(result.r, 1.0);
  EXPECT_DOUBLE_EQ(result.g, 0.0);
  EXPECT_DOUBLE_EQ(result.b, 0.0);
}

TEST(SolidColorTest, SampleIgnoresUVCoordinates) {
  Color blue(0.0, 0.0, 1.0);
  SolidColor texture(blue);

  Color atOrigin = texture.sample(0.0, 0.0, Vector3D(0.0, 0.0, 0.0));
  Color atCorner = texture.sample(1.0, 1.0, Vector3D(100.0, 200.0, 300.0));

  EXPECT_DOUBLE_EQ(atOrigin.r, atCorner.r);
  EXPECT_DOUBLE_EQ(atOrigin.g, atCorner.g);
  EXPECT_DOUBLE_EQ(atOrigin.b, atCorner.b);
}

TEST(SolidColorTest, SampleIgnores3DPoint) {
  Color green(0.0, 1.0, 0.0);
  SolidColor texture(green);

  Color atA = texture.sample(0.5, 0.5, Vector3D(1.0, 2.0, 3.0));
  Color atB = texture.sample(0.5, 0.5, Vector3D(-99.0, 42.0, 0.001));

  EXPECT_DOUBLE_EQ(atA.r, atB.r);
  EXPECT_DOUBLE_EQ(atA.g, atB.g);
  EXPECT_DOUBLE_EQ(atA.b, atB.b);
}
