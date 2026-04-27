/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Optics unit tests
*/

#include <gtest/gtest.h>
#include <cmath>
#include "Math/Optics.hpp"
#include "Math/Vector3D.hpp"

using Math::Vector3D;
namespace Optics = Math::Optics;

TEST(OpticsTest, ReflectHorizontalPlane) {
  Vector3D v(1.0, -1.0, 0.0);
  Vector3D n(0.0, 1.0, 0.0);
  Vector3D r = Optics::reflect(v, n);
  EXPECT_NEAR(r.x, 1.0, 1e-9);
  EXPECT_NEAR(r.y, 1.0, 1e-9);
  EXPECT_NEAR(r.z, 0.0, 1e-9);
}

TEST(OpticsTest, ReflectPerpendicularRay) {
  Vector3D v(0.0, -1.0, 0.0);
  Vector3D n(0.0, 1.0, 0.0);
  Vector3D r = Optics::reflect(v, n);
  EXPECT_NEAR(r.x, 0.0, 1e-9);
  EXPECT_NEAR(r.y, 1.0, 1e-9);
  EXPECT_NEAR(r.z, 0.0, 1e-9);
}

TEST(OpticsTest, RefractDenserMedium) {
  Vector3D v(0.0, -1.0, 0.0);
  Vector3D n(0.0, 1.0, 0.0);
  Vector3D r = Optics::refract(v, n, 1.0 / 1.5);
  EXPECT_NEAR(r.x, 0.0, 1e-9);
  EXPECT_NEAR(r.y, -1.0, 1e-9);
  EXPECT_NEAR(r.z, 0.0, 1e-9);
}

TEST(OpticsTest, RefractTotalInternalReflection) {
  Vector3D v(0.9, -0.1, 0.0);
  Vector3D n(0.0, 1.0, 0.0);
  Vector3D r = Optics::refract(v, n, 1.5);
  Vector3D reflected = Optics::reflect(v, n);
  EXPECT_NEAR(r.x, reflected.x, 1e-9);
  EXPECT_NEAR(r.y, reflected.y, 1e-9);
  EXPECT_NEAR(r.z, reflected.z, 1e-9);
}

TEST(OpticsTest, SchlickAtZeroDegrees) {
  double refIdx = 1.5;
  double r0 = (refIdx - 1.0) / (refIdx + 1.0);
  r0 = r0 * r0;
  EXPECT_NEAR(Optics::schlick(1.0, refIdx), r0, 1e-9);
}

TEST(OpticsTest, SchlickAt90Degrees) {
  EXPECT_NEAR(Optics::schlick(0.0, 1.5), 1.0, 1e-9);
}
