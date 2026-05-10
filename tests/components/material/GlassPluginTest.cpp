/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Glass material plugin integration tests
*/

#include <gtest/gtest.h>
#include "fixture/GlassPluginFixture.hpp"
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
using raytracer::math::Optics::refract;

namespace {

HitRecord makeHitRecord(bool frontFace = true) {
  HitRecord rec;
  rec.point = Vector3D(0.0, 0.0, 0.0);
  rec.normal = Vector3D(0.0, 1.0, 0.0);
  rec.t = 1.0;
  rec.frontFace = frontFace;
  return rec;
}

}  // namespace

TEST_F(GlassPluginFixture, ExposesCreateAndDestroyEntryPoints) {
  ASSERT_NE(createFn_, nullptr);
  ASSERT_NE(destroyFn_, nullptr);
}

TEST_F(GlassPluginFixture, CreateMaterialWithValidRefractionIndex) {
  const double refractionIndex = 1.5;
  IMaterial* glass = makeMaterial(refractionIndex);

  ASSERT_NE(glass, nullptr);
}

TEST_F(GlassPluginFixture, ScatterAttenuationIsWhite) {
  const double refractionIndex = 1.5;
  IMaterial* glass = makeMaterial(refractionIndex);

  HitRecord rec = makeHitRecord(true);
  Ray in(Vector3D(0.0, 2.0, 0.0), Vector3D(0.0, -1.0, 0.0));
  Color attenuation;
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  const bool didScatter = glass->scatter(in, rec, attenuation, scattered);

  EXPECT_TRUE(didScatter);
  EXPECT_DOUBLE_EQ(attenuation.r, 1.0);
  EXPECT_DOUBLE_EQ(attenuation.g, 1.0);
  EXPECT_DOUBLE_EQ(attenuation.b, 1.0);
}

TEST_F(GlassPluginFixture, ScatterProducesRayFromHitPoint) {
  const double refractionIndex = 1.5;
  IMaterial* glass = makeMaterial(refractionIndex);

  const Vector3D hitPoint(1.0, 2.0, 3.0);
  HitRecord rec = makeHitRecord(true);
  rec.point = hitPoint;

  Ray in(Vector3D(0.0, 5.0, 0.0), Vector3D(0.0, -1.0, 0.0));
  Color attenuation;
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  EXPECT_TRUE(glass->scatter(in, rec, attenuation, scattered));

  EXPECT_DOUBLE_EQ(scattered.getOrigin().x, hitPoint.x);
  EXPECT_DOUBLE_EQ(scattered.getOrigin().y, hitPoint.y);
  EXPECT_DOUBLE_EQ(scattered.getOrigin().z, hitPoint.z);
}

TEST_F(GlassPluginFixture, ScatterReturnsTrue) {
  const double refractionIndex = 1.5;
  IMaterial* glass = makeMaterial(refractionIndex);

  HitRecord rec = makeHitRecord(true);
  Ray in(Vector3D(0.0, 1.0, 0.0), Vector3D(0.0, -1.0, 0.0));
  Color attenuation;
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  for (int i = 0; i < 100; ++i) {
    EXPECT_TRUE(glass->scatter(in, rec, attenuation, scattered));
  }
}

TEST_F(GlassPluginFixture, EmittedReturnsBlack) {
  const double refractionIndex = 1.5;
  IMaterial* glass = makeMaterial(refractionIndex);

  const Color emitted = glass->emitted();

  EXPECT_DOUBLE_EQ(emitted.r, 0.0);
  EXPECT_DOUBLE_EQ(emitted.g, 0.0);
  EXPECT_DOUBLE_EQ(emitted.b, 0.0);
}

TEST_F(GlassPluginFixture, TotalInternalReflectionAtGrazingAngle) {
  const double refractionIndex = 1.5;
  IMaterial* glass = makeMaterial(refractionIndex);

  HitRecord rec = makeHitRecord(false);
  Ray in(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 1.0, 0.0).normalize());
  Color attenuation;
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  const bool didScatter = glass->scatter(in, rec, attenuation, scattered);

  EXPECT_TRUE(didScatter);
  EXPECT_DOUBLE_EQ(attenuation.r, 1.0);
  EXPECT_DOUBLE_EQ(attenuation.g, 1.0);
  EXPECT_DOUBLE_EQ(attenuation.b, 1.0);
}

TEST_F(GlassPluginFixture, RefractionOccursAtNormalIncidence) {
  const double refractionIndex = 1.5;
  IMaterial* glass = makeMaterial(refractionIndex);

  HitRecord rec = makeHitRecord(true);
  Ray in(Vector3D(0.0, 10.0, 0.0), Vector3D(0.0, -1.0, 0.0));
  Color attenuation;
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  EXPECT_TRUE(glass->scatter(in, rec, attenuation, scattered));
  EXPECT_DOUBLE_EQ(scattered.getOrigin().x, 0.0);
  EXPECT_DOUBLE_EQ(scattered.getOrigin().y, 0.0);
  EXPECT_DOUBLE_EQ(scattered.getOrigin().z, 0.0);
}

TEST_F(GlassPluginFixture, DifferentRefractionIndices) {
  const double lowRefraction = 1.0;
  const double highRefraction = 2.5;
  IMaterial* glassLow = makeMaterial(lowRefraction);
  IMaterial* glassHigh = makeMaterial(highRefraction);

  HitRecord rec = makeHitRecord(true);
  Ray in(Vector3D(0.0, 2.0, 0.0), Vector3D(0.0, -1.0, 0.0));
  Color attenuation1, attenuation2;
  Ray scattered1(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));
  Ray scattered2(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));

  EXPECT_TRUE(glassLow->scatter(in, rec, attenuation1, scattered1));
  EXPECT_TRUE(glassHigh->scatter(in, rec, attenuation2, scattered2));
}
