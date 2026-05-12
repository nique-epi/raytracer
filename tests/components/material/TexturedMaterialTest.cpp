/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TexturedMaterial unit tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "components/material/Textures/solid/SolidColor.hpp"
#include "components/material/textured/TexturedMaterial.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::components::material::TexturedMaterial;
using raytracer::materials::textures::SolidColor;
using raytracer::math::Color;
using raytracer::math::HitRecord;
using raytracer::math::Ray;
using raytracer::math::Vector3D;

namespace {

HitRecord makeHitRecord(double u, double v, const Vector3D& point,
                        const Vector3D& normal) {
  HitRecord rec;
  rec.u = u;
  rec.v = v;
  rec.point = point;
  rec.normal = normal;
  rec.frontFace = true;
  return rec;
}

}  // namespace

TEST(TexturedMaterialTest, ScatterAlwaysReturnsTrue) {
  auto texture = std::make_shared<SolidColor>(Color(1.0, 0.0, 0.0));
  TexturedMaterial material(texture, Color(1.0, 1.0, 1.0));

  HitRecord rec =
      makeHitRecord(0.5, 0.5, Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0));
  Ray in(Vector3D(0.0, 1.0, 0.0), Vector3D(0.0, -1.0, 0.0));
  Color attenuation;
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0));

  EXPECT_TRUE(material.scatter(in, rec, attenuation, scattered));
}

TEST(TexturedMaterialTest, AttenuationIsTextureSampleTimesAlbedo) {
  Color textureColor(0.5, 0.8, 0.2);
  Color albedo(1.0, 0.5, 1.0);
  auto texture = std::make_shared<SolidColor>(textureColor);
  TexturedMaterial material(texture, albedo);

  HitRecord rec =
      makeHitRecord(0.0, 0.0, Vector3D(1.0, 2.0, 3.0), Vector3D(0.0, 1.0, 0.0));
  Ray in(Vector3D(0.0, 1.0, 0.0), Vector3D(0.0, -1.0, 0.0));
  Color attenuation;
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0));

  material.scatter(in, rec, attenuation, scattered);

  Color expected = textureColor * albedo;
  EXPECT_NEAR(attenuation.r, expected.r, 1e-9);
  EXPECT_NEAR(attenuation.g, expected.g, 1e-9);
  EXPECT_NEAR(attenuation.b, expected.b, 1e-9);
}

TEST(TexturedMaterialTest, EmittedReturnsBlack) {
  auto texture = std::make_shared<SolidColor>(Color(1.0, 0.0, 0.0));
  TexturedMaterial material(texture, Color(1.0, 1.0, 1.0));

  const Color emitted = material.emitted();
  EXPECT_DOUBLE_EQ(emitted.r, 0.0);
  EXPECT_DOUBLE_EQ(emitted.g, 0.0);
  EXPECT_DOUBLE_EQ(emitted.b, 0.0);
}

TEST(TexturedMaterialTest, ScatteredRayOriginatesAtHitPoint) {
  auto texture = std::make_shared<SolidColor>(Color(1.0, 1.0, 1.0));
  TexturedMaterial material(texture, Color(1.0, 1.0, 1.0));

  Vector3D hitPoint(3.0, 4.0, 5.0);
  HitRecord rec = makeHitRecord(0.0, 0.0, hitPoint, Vector3D(0.0, 1.0, 0.0));
  Ray in(Vector3D(0.0, 5.0, 5.0), Vector3D(0.0, -1.0, 0.0));
  Color attenuation;
  Ray scattered(Vector3D(0.0, 0.0, 0.0), Vector3D(0.0, 1.0, 0.0));

  material.scatter(in, rec, attenuation, scattered);

  EXPECT_NEAR(scattered.getOrigin().x, hitPoint.x, 1e-9);
  EXPECT_NEAR(scattered.getOrigin().y, hitPoint.y, 1e-9);
  EXPECT_NEAR(scattered.getOrigin().z, hitPoint.z, 1e-9);
}
