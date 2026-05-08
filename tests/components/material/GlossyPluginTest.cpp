/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Glossy material plugin integration tests
*/

#include <gtest/gtest.h>
#include "fixture/GlossyPluginFixture.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Optics.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::math::Color;
using raytracer::math::HitRecord;
using raytracer::math::Ray;
using raytracer::math::Vector3D;
using raytracer::math::Optics::reflect;

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

TEST_F(GlossyPluginFixture, ExposesCreateAndDestroyEntryPoints) {
  ASSERT_NE(createFn_, nullptr);
  ASSERT_NE(destroyFn_, nullptr);
}

TEST_F(GlossyPluginFixture, ScatterAttenuationEqualsAlbedo) {
  const Color albedo(0.25, 0.5, 0.75);
  IMaterial* material = makeMaterial(0.0, albedo);

  HitRecord rec = makeHitRecord();
  Ray in(Vector3D(0.0, 2.0, 0.0), Vector3D(0.0, -1.0, 0.0));
  Color attenuation;
  Ray scattered(Vector3D(1.0, 1.0, 1.0), Vector3D(1.0, 0.0, 0.0));

  const bool didScatter = material->scatter(in, rec, attenuation, scattered);

  EXPECT_TRUE(didScatter);
  EXPECT_DOUBLE_EQ(attenuation.r, albedo.r);
  EXPECT_DOUBLE_EQ(attenuation.g, albedo.g);
  EXPECT_DOUBLE_EQ(attenuation.b, albedo.b);
}

TEST_F(GlossyPluginFixture, ScatterProducesRayFromHitPoint) {
  const Color albedo(1.0, 1.0, 1.0);
  const Vector3D hitPoint(1.0, 2.0, 3.0);
  IMaterial* material = makeMaterial(0.0, albedo);

  HitRecord rec = makeHitRecord();
  rec.point = hitPoint;

  Ray in(Vector3D(0.0, 5.0, 0.0), Vector3D(0.0, -1.0, 0.0));
  Color attenuation;
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  EXPECT_TRUE(material->scatter(in, rec, attenuation, scattered));

  EXPECT_DOUBLE_EQ(scattered.getOrigin().x, hitPoint.x);
  EXPECT_DOUBLE_EQ(scattered.getOrigin().y, hitPoint.y);
  EXPECT_DOUBLE_EQ(scattered.getOrigin().z, hitPoint.z);
}

TEST_F(GlossyPluginFixture, ScatterReturnsPerfectReflectionAtZeroFuzz) {
  const Color albedo(1.0, 1.0, 1.0);
  IMaterial* material = makeMaterial(0.0, albedo);

  HitRecord rec = makeHitRecord();
  Ray in(Vector3D(0.0, 1.0, 0.0), Vector3D(0.0, -1.0, 0.0));
  Color attenuation;
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  EXPECT_TRUE(material->scatter(in, rec, attenuation, scattered));

  const Vector3D expectedDirection =
      reflect(in.getDirection().normalize(), rec.normal);

  EXPECT_DOUBLE_EQ(scattered.getDirection().x, expectedDirection.x);
  EXPECT_DOUBLE_EQ(scattered.getDirection().y, expectedDirection.y);
  EXPECT_DOUBLE_EQ(scattered.getDirection().z, expectedDirection.z);
}

TEST_F(GlossyPluginFixture, ScatterReturnsFalseWhenReflectionGoesBelowSurface) {
  const Color albedo(0.25, 0.5, 0.75);
  IMaterial* material = makeMaterial(0.0, albedo);

  HitRecord rec = makeHitRecord();
  Ray in(Vector3D(0.0, 1.0, 0.0), Vector3D(0.0, 1.0, 0.0));
  Color attenuation(0.12, 0.34, 0.56);
  Ray scattered(Vector3D(9.0, 8.0, 7.0), Vector3D(6.0, 5.0, 4.0));

  const bool didScatter = material->scatter(in, rec, attenuation, scattered);

  EXPECT_FALSE(didScatter);
  EXPECT_DOUBLE_EQ(attenuation.r, 0.12);
  EXPECT_DOUBLE_EQ(attenuation.g, 0.34);
  EXPECT_DOUBLE_EQ(attenuation.b, 0.56);
  EXPECT_DOUBLE_EQ(scattered.getOrigin().x, 9.0);
  EXPECT_DOUBLE_EQ(scattered.getOrigin().y, 8.0);
  EXPECT_DOUBLE_EQ(scattered.getOrigin().z, 7.0);
  EXPECT_DOUBLE_EQ(scattered.getDirection().x, 6.0);
  EXPECT_DOUBLE_EQ(scattered.getDirection().y, 5.0);
  EXPECT_DOUBLE_EQ(scattered.getDirection().z, 4.0);
}

TEST_F(GlossyPluginFixture, NegativeFuzzClampsToZero) {
  const Color albedo(0.8, 0.2, 0.6);
  IMaterial* negative = makeMaterial(-10.0, albedo);
  IMaterial* zero = makeMaterial(0.0, albedo);

  HitRecord rec = makeHitRecord();
  Ray in(Vector3D(0.0, 4.0, 0.0), Vector3D(0.0, -1.0, 0.0));

  Color attenuationNegative;
  Color attenuationZero;
  Ray scatteredNegative(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));
  Ray scatteredZero(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  EXPECT_TRUE(
      negative->scatter(in, rec, attenuationNegative, scatteredNegative));
  EXPECT_TRUE(zero->scatter(in, rec, attenuationZero, scatteredZero));

  EXPECT_DOUBLE_EQ(attenuationNegative.r, attenuationZero.r);
  EXPECT_DOUBLE_EQ(attenuationNegative.g, attenuationZero.g);
  EXPECT_DOUBLE_EQ(attenuationNegative.b, attenuationZero.b);
  EXPECT_DOUBLE_EQ(scatteredNegative.getOrigin().x,
                   scatteredZero.getOrigin().x);
  EXPECT_DOUBLE_EQ(scatteredNegative.getOrigin().y,
                   scatteredZero.getOrigin().y);
  EXPECT_DOUBLE_EQ(scatteredNegative.getOrigin().z,
                   scatteredZero.getOrigin().z);
  EXPECT_DOUBLE_EQ(scatteredNegative.getDirection().x,
                   scatteredZero.getDirection().x);
  EXPECT_DOUBLE_EQ(scatteredNegative.getDirection().y,
                   scatteredZero.getDirection().y);
  EXPECT_DOUBLE_EQ(scatteredNegative.getDirection().z,
                   scatteredZero.getDirection().z);
}
