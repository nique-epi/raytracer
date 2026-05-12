/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Color unit tests
*/

#include <gtest/gtest.h>
#include <cmath>
#include <tuple>
#include "exceptions/Exceptions.hpp"
#include "utils/math/Color.hpp"

using raytracer::math::Color;

TEST(ColorTest, DefaultConstructorIsBlack) {
  Color c;
  EXPECT_DOUBLE_EQ(c.r, 0.0);
  EXPECT_DOUBLE_EQ(c.g, 0.0);
  EXPECT_DOUBLE_EQ(c.b, 0.0);
}

TEST(ColorTest, ParameterizedConstructor) {
  Color c(0.1, 0.5, 0.9);
  EXPECT_DOUBLE_EQ(c.r, 0.1);
  EXPECT_DOUBLE_EQ(c.g, 0.5);
  EXPECT_DOUBLE_EQ(c.b, 0.9);
}

TEST(ColorTest, Add) {
  Color a(0.1, 0.2, 0.3);
  Color b(0.4, 0.5, 0.6);
  Color c = a + b;
  EXPECT_NEAR(c.r, 0.5, 1e-9);
  EXPECT_NEAR(c.g, 0.7, 1e-9);
  EXPECT_NEAR(c.b, 0.9, 1e-9);
}

TEST(ColorTest, Subtract) {
  Color a(0.8, 0.7, 0.6);
  Color b(0.1, 0.2, 0.3);
  Color c = a - b;
  EXPECT_NEAR(c.r, 0.7, 1e-9);
  EXPECT_NEAR(c.g, 0.5, 1e-9);
  EXPECT_NEAR(c.b, 0.3, 1e-9);
}

TEST(ColorTest, MultiplyColor) {
  Color a(0.5, 0.4, 0.2);
  Color b(0.2, 0.5, 1.0);
  Color c = a * b;
  EXPECT_NEAR(c.r, 0.1, 1e-9);
  EXPECT_NEAR(c.g, 0.2, 1e-9);
  EXPECT_NEAR(c.b, 0.2, 1e-9);
}

TEST(ColorTest, MultiplyScalar) {
  Color a(0.5, 0.5, 0.5);
  Color c = a * 2.0;
  EXPECT_NEAR(c.r, 1.0, 1e-9);
  EXPECT_NEAR(c.g, 1.0, 1e-9);
  EXPECT_NEAR(c.b, 1.0, 1e-9);
}

TEST(ColorTest, MultiplyScalarCommutative) {
  Color a(0.5, 0.5, 0.5);
  Color c = 2.0 * a;
  EXPECT_NEAR(c.r, 1.0, 1e-9);
}

TEST(ColorTest, DivideScalar) {
  Color a(1.0, 0.5, 0.2);
  Color c = a / 2.0;
  EXPECT_NEAR(c.r, 0.5, 1e-9);
  EXPECT_NEAR(c.g, 0.25, 1e-9);
  EXPECT_NEAR(c.b, 0.1, 1e-9);
}

TEST(ColorTest, AddAssign) {
  Color a(0.1, 0.2, 0.3);
  a += Color(0.1, 0.1, 0.1);
  EXPECT_NEAR(a.r, 0.2, 1e-9);
  EXPECT_NEAR(a.g, 0.3, 1e-9);
  EXPECT_NEAR(a.b, 0.4, 1e-9);
}

TEST(ColorTest, MultiplyAssign) {
  Color a(0.5, 0.5, 0.5);
  a *= 2.0;
  EXPECT_NEAR(a.r, 1.0, 1e-9);
}

TEST(ColorTest, ClampBelowZero) {
  Color c(-1.0, 0.5, 2.0);
  Color clamped = c.clamp();
  EXPECT_DOUBLE_EQ(clamped.r, 0.0);
  EXPECT_DOUBLE_EQ(clamped.g, 0.5);
  EXPECT_DOUBLE_EQ(clamped.b, 1.0);
}

TEST(ColorTest, ClampAlreadyValid) {
  Color c(0.2, 0.5, 0.8);
  Color clamped = c.clamp();
  EXPECT_DOUBLE_EQ(clamped.r, 0.2);
  EXPECT_DOUBLE_EQ(clamped.g, 0.5);
  EXPECT_DOUBLE_EQ(clamped.b, 0.8);
}

TEST(ColorTest, GammaCorrectInvalidThrows) {
  Color c(0.5, 0.5, 0.5);
  EXPECT_THROW(
      {
        auto _ = c.gammaCorrect(0.0);
        (void)_;
      },
      raytracer::core::RaytracerException);
  EXPECT_THROW(
      {
        auto _ = c.gammaCorrect(-1.0);
        (void)_;
      },
      raytracer::core::RaytracerException);
}

TEST(ColorTest, GammaCorrect22) {
  Color c(0.5, 0.5, 0.5);
  Color gc = c.gammaCorrect(2.2);
  double expected = std::pow(0.5, 1.0 / 2.2);
  EXPECT_NEAR(gc.r, expected, 1e-9);
  EXPECT_NEAR(gc.g, expected, 1e-9);
  EXPECT_NEAR(gc.b, expected, 1e-9);
}

TEST(ColorTest, ToRGB) {
  Color c(1.0, 0.5, 0.0);
  auto [r, g, b] = c.toRGB();
  EXPECT_EQ(r, 255);
  EXPECT_EQ(g, 127);
  EXPECT_EQ(b, 0);
}

TEST(ColorTest, ToRGBClampsOverflow) {
  Color c(2.0, -1.0, 0.5);
  auto [r, g, b] = c.toRGB();
  EXPECT_EQ(r, 255);
  EXPECT_EQ(g, 0);
  EXPECT_EQ(b, 127);
}
