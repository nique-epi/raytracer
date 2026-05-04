/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Diffuse material plugin integration tests
*/

#include <gtest/gtest.h>
#include "fixture/DiffusePluginFixture.hpp"
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

TEST_F(DiffusePluginFixture, ExposesCreateAndDestroyEntryPoints) {
  IMaterial* another = createFn();
  ASSERT_NE(another, nullptr);
  destroyFn(another);
}

TEST_F(DiffusePluginFixture, ScatterAttenuationEqualsAlbedo) {
  const Color expectedAlbedo(1.0, 1.0, 1.0);

  HitRecord rec = makeHitRecord();
  Ray in(Vector3D(0.0, 2.0, 0.0), Vector3D(0.0, -1.0, 0.0));
  Color attenuation;
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  const bool did = material->scatter(in, rec, attenuation, scattered);

  EXPECT_TRUE(did);
  EXPECT_DOUBLE_EQ(attenuation.r, expectedAlbedo.r);
  EXPECT_DOUBLE_EQ(attenuation.g, expectedAlbedo.g);
  EXPECT_DOUBLE_EQ(attenuation.b, expectedAlbedo.b);
}

TEST_F(DiffusePluginFixture, ScatterProducesRayFromHitPoint) {
  const Vector3D hitPoint(1.0, 2.0, 3.0);
  HitRecord rec = makeHitRecord();
  rec.point = hitPoint;

  Ray in(Vector3D(0.0, 5.0, 0.0), Vector3D(0.0, -1.0, 0.0));
  Color attenuation;
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  material->scatter(in, rec, attenuation, scattered);

  EXPECT_DOUBLE_EQ(scattered.getOrigin().x, hitPoint.x);
  EXPECT_DOUBLE_EQ(scattered.getOrigin().y, hitPoint.y);
  EXPECT_DOUBLE_EQ(scattered.getOrigin().z, hitPoint.z);
}

TEST_F(DiffusePluginFixture, EmittedReturnsBlack) {
  const Color emitted = material->emitted();

  EXPECT_DOUBLE_EQ(emitted.r, 0.0);
  EXPECT_DOUBLE_EQ(emitted.g, 0.0);
  EXPECT_DOUBLE_EQ(emitted.b, 0.0);
}

TEST_F(DiffusePluginFixture, ScatterReturnsTrueAlways) {
  HitRecord rec = makeHitRecord();
  Ray in(Vector3D(0.0, 1.0, 0.0), Vector3D(0.0, -1.0, 0.0));
  Color attenuation;
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  for (int i = 0; i < 100; ++i) {
    EXPECT_TRUE(material->scatter(in, rec, attenuation, scattered));
  }
}
