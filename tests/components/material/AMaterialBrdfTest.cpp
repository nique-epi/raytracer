/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** AMaterial::brdf unit tests (universal Phong reflection model)
*/

#include <gtest/gtest.h>
#include <cmath>
#include "components/material/abstract/AMaterial.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Constants.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::math::Color;
using raytracer::math::HitRecord;
using raytracer::math::Ray;
using raytracer::math::Vector3D;

namespace {

constexpr double tolerance = 1e-9;

class TestableMaterial : public AMaterial {
 public:
  TestableMaterial(const Color& diffuse, const Color& specularAlbedo,
                   double shininess)
      : AMaterial(specularAlbedo, shininess), diffuse_(diffuse) {}

  bool scatter(const Ray& /*in*/, const HitRecord& /*rec*/,
               Color& /*attenuation*/, Ray& /*scattered*/) const override {
    return false;
  }

  [[nodiscard]] Color diffuseAlbedo() const override { return diffuse_; }

 private:
  Color diffuse_;
};

Color expectedLambert(const Color& albedo) {
  return albedo / raytracer::math::constants::PI;
}

}  // namespace

TEST(AMaterialBrdfTest, DefaultsProduceZeroBrdf) {
  TestableMaterial material(Color(0.0, 0.0, 0.0), Color(0.0, 0.0, 0.0), 1.0);

  const Color brdf = material.brdf(Vector3D(0.0, -1.0, 0.0),
                                    Vector3D(0.0, 1.0, 0.0),
                                    Vector3D(0.0, 1.0, 0.0));

  EXPECT_NEAR(brdf.r, 0.0, tolerance);
  EXPECT_NEAR(brdf.g, 0.0, tolerance);
  EXPECT_NEAR(brdf.b, 0.0, tolerance);
}

TEST(AMaterialBrdfTest, PureLambertWhenSpecularIsBlack) {
  const Color albedo(0.5, 0.5, 0.5);
  TestableMaterial material(albedo, Color(0.0, 0.0, 0.0), 32.0);

  const Color brdf = material.brdf(Vector3D(0.0, -1.0, 0.0),
                                    Vector3D(0.0, 1.0, 0.0),
                                    Vector3D(0.0, 1.0, 0.0));

  const Color expected = expectedLambert(albedo);
  EXPECT_NEAR(brdf.r, expected.r, tolerance);
  EXPECT_NEAR(brdf.g, expected.g, tolerance);
  EXPECT_NEAR(brdf.b, expected.b, tolerance);
}

TEST(AMaterialBrdfTest, PureSpecularAlignedWithReflection) {
  const Color specularAlbedo(1.0, 1.0, 1.0);
  TestableMaterial material(Color(0.0, 0.0, 0.0), specularAlbedo, 8.0);

  const Vector3D normal(0.0, 1.0, 0.0);
  const Vector3D incoming(0.0, -1.0, 0.0);
  const Vector3D outgoing(0.0, 1.0, 0.0);

  const Color brdf = material.brdf(incoming, outgoing, normal);

  EXPECT_NEAR(brdf.r, 1.0, tolerance);
  EXPECT_NEAR(brdf.g, 1.0, tolerance);
  EXPECT_NEAR(brdf.b, 1.0, tolerance);
}

TEST(AMaterialBrdfTest, SpecularOutOfHighlightConeIsZero) {
  TestableMaterial material(Color(0.0, 0.0, 0.0), Color(1.0, 1.0, 1.0), 32.0);

  const Vector3D normal(0.0, 1.0, 0.0);
  const Vector3D incoming(0.0, -1.0, 0.0);
  const Vector3D outgoing(1.0, 0.0, 0.0);

  const Color brdf = material.brdf(incoming, outgoing, normal);

  EXPECT_NEAR(brdf.r, 0.0, tolerance);
  EXPECT_NEAR(brdf.g, 0.0, tolerance);
  EXPECT_NEAR(brdf.b, 0.0, tolerance);
}

TEST(AMaterialBrdfTest, HighShininessNarrowsHighlight) {
  const Color specularAlbedo(1.0, 1.0, 1.0);
  TestableMaterial wide(Color(0.0, 0.0, 0.0), specularAlbedo, 4.0);
  TestableMaterial narrow(Color(0.0, 0.0, 0.0), specularAlbedo, 512.0);

  const Vector3D normal(0.0, 1.0, 0.0);
  const Vector3D incoming(0.0, -1.0, 0.0);
  const double sinTheta = std::sqrt(0.19);
  const Vector3D outgoing(sinTheta, 0.9, 0.0);

  const Color wideBrdf = wide.brdf(incoming, outgoing, normal);
  const Color narrowBrdf = narrow.brdf(incoming, outgoing, normal);

  EXPECT_NEAR(wideBrdf.r, std::pow(0.9, 4.0), tolerance);
  EXPECT_LT(narrowBrdf.r, 1e-10);
}

TEST(AMaterialBrdfTest, LambertAndPhongCompose) {
  const Color albedo(0.4, 0.4, 0.4);
  const Color specularAlbedo(0.2, 0.2, 0.2);
  TestableMaterial material(albedo, specularAlbedo, 8.0);

  const Vector3D normal(0.0, 1.0, 0.0);
  const Vector3D incoming(0.0, -1.0, 0.0);
  const Vector3D outgoing(0.0, 1.0, 0.0);

  const Color brdf = material.brdf(incoming, outgoing, normal);

  const Color expected = expectedLambert(albedo) + specularAlbedo;
  EXPECT_NEAR(brdf.r, expected.r, tolerance);
  EXPECT_NEAR(brdf.g, expected.g, tolerance);
  EXPECT_NEAR(brdf.b, expected.b, tolerance);
}
