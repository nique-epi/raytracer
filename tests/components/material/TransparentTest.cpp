/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Transparent material unit tests
*/

#include <gtest/gtest.h>
#include "components/material/transparent/Transparent.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::components::material::Transparent;
using raytracer::math::Color;
using raytracer::math::HitRecord;
using raytracer::math::Ray;
using raytracer::math::Vector3D;

namespace {

constexpr double tolerance = 1e-6;

HitRecord makeHitRecord(const Vector3D& point, const Vector3D& normal) {
  HitRecord rec;
  rec.point = point;
  rec.normal = normal;
  rec.frontFace = true;
  rec.t = 1.0;
  return rec;
}

}  // namespace

TEST(TransparentTest, ScatterPreservesIncomingDirection) {
  const Transparent material;
  const Vector3D incomingDirection(1.0, 0.0, 0.0);
  const Ray incoming(Vector3D(-1.0, 0.0, 0.0), incomingDirection);
  const HitRecord rec =
      makeHitRecord(Vector3D(0.0, 0.0, 0.0), Vector3D(-1.0, 0.0, 0.0));
  Color attenuation(0.0, 0.0, 0.0);
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  EXPECT_TRUE(material.scatter(incoming, rec, attenuation, scattered));

  const Vector3D scatteredDirection = scattered.getDirection();
  EXPECT_NEAR(scatteredDirection.x, 1.0, tolerance);
  EXPECT_NEAR(scatteredDirection.y, 0.0, tolerance);
  EXPECT_NEAR(scatteredDirection.z, 0.0, tolerance);
}

TEST(TransparentTest, ScatterOffsetsOriginPastSurface) {
  const Transparent material;
  const Ray incoming(Vector3D(0.0, 0.0, -1.0), Vector3D(0.0, 0.0, 1.0));
  const HitRecord rec =
      makeHitRecord(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, -1.0));
  Color attenuation(0.0, 0.0, 0.0);
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  material.scatter(incoming, rec, attenuation, scattered);

  EXPECT_GT(scattered.getOrigin().z, 0.0);
}

TEST(TransparentTest, ScatterAttenuationEqualsTint) {
  const Color tint(0.4, 0.7, 0.2);
  const Transparent material(tint);
  const Ray incoming(Vector3D(0.0, 0.0, -1.0), Vector3D(0.0, 0.0, 1.0));
  const HitRecord rec =
      makeHitRecord(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, -1.0));
  Color attenuation(0.0, 0.0, 0.0);
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  material.scatter(incoming, rec, attenuation, scattered);

  EXPECT_NEAR(attenuation.r, tint.r, tolerance);
  EXPECT_NEAR(attenuation.g, tint.g, tolerance);
  EXPECT_NEAR(attenuation.b, tint.b, tolerance);
}

TEST(TransparentTest, DefaultTintIsNeutralWhite) {
  const Transparent material;

  const Color& tint = material.tint();

  EXPECT_NEAR(tint.r, 1.0, tolerance);
  EXPECT_NEAR(tint.g, 1.0, tolerance);
  EXPECT_NEAR(tint.b, 1.0, tolerance);
}

TEST(TransparentTest, DiffuseAlbedoIsBlackToSkipDirectLighting) {
  const Transparent material(Color(0.4, 0.7, 0.2));

  const Color diffuseAlbedo = material.diffuseAlbedo();

  EXPECT_DOUBLE_EQ(diffuseAlbedo.r, 0.0);
  EXPECT_DOUBLE_EQ(diffuseAlbedo.g, 0.0);
  EXPECT_DOUBLE_EQ(diffuseAlbedo.b, 0.0);
}

TEST(TransparentTest, EmittedIsBlack) {
  const Transparent material(Color(0.5, 0.5, 0.5));

  const Color emitted = material.emitted();

  EXPECT_DOUBLE_EQ(emitted.r, 0.0);
  EXPECT_DOUBLE_EQ(emitted.g, 0.0);
  EXPECT_DOUBLE_EQ(emitted.b, 0.0);
}
