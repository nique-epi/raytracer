/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** GradientBackgroundTest
*/

#include <gtest/gtest.h>
#include "scene/background/Gradient/GradientBackground.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::math::Color;
using raytracer::math::Ray;
using raytracer::math::Vector3D;
using raytracer::scene::background::GradientBackground;

TEST(GradientBackgroundTest, InterpolatesBetweenTopAndBottom) {
  Color top(1.0, 0.0, 0.0);     // red
  Color bottom(0.0, 0.0, 1.0);  // blue
  GradientBackground bg(top, bottom);

  Ray rTop(Vector3D(0, 0, 0), Vector3D(0, -1, 0));
  auto colTop = bg.getColor(rTop);
  EXPECT_DOUBLE_EQ(colTop.r, top.r);
  EXPECT_DOUBLE_EQ(colTop.g, top.g);
  EXPECT_DOUBLE_EQ(colTop.b, top.b);

  Ray rBottom(Vector3D(0, 0, 0), Vector3D(0, 1, 0));
  auto colBottom = bg.getColor(rBottom);
  EXPECT_DOUBLE_EQ(colBottom.r, bottom.r);
  EXPECT_DOUBLE_EQ(colBottom.g, bottom.g);
  EXPECT_DOUBLE_EQ(colBottom.b, bottom.b);

  Ray rMid(Vector3D(0, 0, 0), Vector3D(0, 0, 1));
  auto colMid = bg.getColor(rMid);
  EXPECT_DOUBLE_EQ(colMid.r, 0.5 * (top.r + bottom.r));
  EXPECT_DOUBLE_EQ(colMid.g, 0.5 * (top.g + bottom.g));
  EXPECT_DOUBLE_EQ(colMid.b, 0.5 * (top.b + bottom.b));
}
