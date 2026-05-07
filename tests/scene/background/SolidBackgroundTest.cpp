/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SolidBackgroundTest
*/

#include <gtest/gtest.h>
#include "scene/background/Solid/SolidBackground.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::math::Color;
using raytracer::math::Ray;
using raytracer::math::Vector3D;
using raytracer::scene::background::SolidBackground;

TEST(SolidBackgroundTest, ReturnsSameColorForDifferentRays) {
  Color c(0.2, 0.4, 0.6);
  SolidBackground bg(c);

  Ray r1(Vector3D(0, 0, 0), Vector3D(1, 0, 0));
  Ray r2(Vector3D(0, 0, 0), Vector3D(0, 1, 1));

  auto col1 = bg.getColor(r1);
  auto col2 = bg.getColor(r2);

  EXPECT_DOUBLE_EQ(col1.r, c.r);
  EXPECT_DOUBLE_EQ(col1.g, c.g);
  EXPECT_DOUBLE_EQ(col1.b, c.b);

  EXPECT_DOUBLE_EQ(col2.r, c.r);
  EXPECT_DOUBLE_EQ(col2.g, c.g);
  EXPECT_DOUBLE_EQ(col2.b, c.b);
}
