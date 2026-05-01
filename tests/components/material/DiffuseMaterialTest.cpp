/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** DiffuseMaterial unit tests
*/

#include <gtest/gtest.h>
#include "components/material/diffuse/DiffuseMaterial.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::math::Color;
using raytracer::math::HitRecord;
using raytracer::math::Ray;
using raytracer::math::Vector3D;

namespace {

HitRecord makeHitRecord() {
  HitRecord rec;
  rec.point = Vector3D(0.0, 0.0, 0.0);
  rec.normal = Vector3D(0.0, 1.0, 0.0);
  rec.t = 1.0;
  rec.frontFace = true;
  return rec;
}

}  // namespace

TEST(DiffuseMaterialTest, ScatterAttenuationEqualsAlbedo) {
  const Color albedo(0.8, 0.5, 0.2);
  DiffuseMaterial mat(albedo);

  Ray in(Vector3D(0.0, 2.0, 0.0), Vector3D(0.0, -1.0, 0.0));
  HitRecord rec = makeHitRecord();
  Color attenuation;
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  const bool did = mat.scatter(in, rec, attenuation, scattered);

  EXPECT_TRUE(did);
  EXPECT_DOUBLE_EQ(attenuation.r, albedo.r);
  EXPECT_DOUBLE_EQ(attenuation.g, albedo.g);
  EXPECT_DOUBLE_EQ(attenuation.b, albedo.b);
}

TEST(DiffuseMaterialTest, ScatterProducesRayFromHitPoint) {
  DiffuseMaterial mat(Color(1.0, 1.0, 1.0));

  const Vector3D hitPoint(1.0, 2.0, 3.0);
  Ray in(Vector3D(0.0, 5.0, 0.0), Vector3D(0.0, -1.0, 0.0));
  HitRecord rec = makeHitRecord();
  rec.point = hitPoint;
  Color attenuation;
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  mat.scatter(in, rec, attenuation, scattered);

  EXPECT_DOUBLE_EQ(scattered.getOrigin().x, hitPoint.x);
  EXPECT_DOUBLE_EQ(scattered.getOrigin().y, hitPoint.y);
  EXPECT_DOUBLE_EQ(scattered.getOrigin().z, hitPoint.z);
}

TEST(DiffuseMaterialTest, EmittedReturnsBlack) {
  DiffuseMaterial mat(Color(0.9, 0.3, 0.7));

  const Color emitted = mat.emitted();

  EXPECT_DOUBLE_EQ(emitted.r, 0.0);
  EXPECT_DOUBLE_EQ(emitted.g, 0.0);
  EXPECT_DOUBLE_EQ(emitted.b, 0.0);
}

TEST(DiffuseMaterialTest, ScatterReturnsTrueAlways) {
  DiffuseMaterial mat(Color(0.5, 0.5, 0.5));

  Ray in(Vector3D(0.0, 1.0, 0.0), Vector3D(0.0, -1.0, 0.0));
  HitRecord rec = makeHitRecord();
  Color attenuation;
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  for (int i = 0; i < 100; ++i) {
    EXPECT_TRUE(mat.scatter(in, rec, attenuation, scattered));
  }
}
