/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PrincipledMaterial direct-lighting BRDF unit tests
*/

#include <gtest/gtest.h>
#include <cmath>
#include "components/material/principledBSDF/PrincipledBSDF.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Constants.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::components::material::PrincipledMaterial;
using raytracer::math::Color;
using raytracer::math::Vector3D;

namespace {

constexpr double tolerance = 1e-9;
constexpr double dielectricF0 = 0.04;

}  // namespace

TEST(PrincipledBrdfTest, PureMetalReturnsBaseColorAsF0) {
  PrincipledMaterial material(Color(0.8, 0.5, 0.2), /*metallic=*/1.0,
                               /*roughness=*/0.5);

  const Color specularAlbedo = material.specularAlbedo();

  EXPECT_NEAR(specularAlbedo.r, 0.8, tolerance);
  EXPECT_NEAR(specularAlbedo.g, 0.5, tolerance);
  EXPECT_NEAR(specularAlbedo.b, 0.2, tolerance);
}

TEST(PrincipledBrdfTest, PureDielectricReturnsConstantF0) {
  PrincipledMaterial material(Color(1.0, 0.0, 0.0), /*metallic=*/0.0,
                               /*roughness=*/0.5);

  const Color specularAlbedo = material.specularAlbedo();

  EXPECT_NEAR(specularAlbedo.r, dielectricF0, tolerance);
  EXPECT_NEAR(specularAlbedo.g, dielectricF0, tolerance);
  EXPECT_NEAR(specularAlbedo.b, dielectricF0, tolerance);
}

TEST(PrincipledBrdfTest, MetallicHalfMixesF0Linearly) {
  PrincipledMaterial material(Color(1.0, 1.0, 1.0), /*metallic=*/0.5,
                               /*roughness=*/0.5);

  const Color specularAlbedo = material.specularAlbedo();

  const double expected = (dielectricF0 * 0.5) + (1.0 * 0.5);
  EXPECT_NEAR(specularAlbedo.r, expected, tolerance);
  EXPECT_NEAR(specularAlbedo.g, expected, tolerance);
  EXPECT_NEAR(specularAlbedo.b, expected, tolerance);
}

TEST(PrincipledBrdfTest, ZeroRoughnessProducesMaxShininess) {
  PrincipledMaterial material(Color(1.0, 1.0, 1.0), /*metallic=*/0.0,
                               /*roughness=*/0.0);

  EXPECT_NEAR(material.shininess(), 256.0, tolerance);
}

TEST(PrincipledBrdfTest, FullRoughnessProducesMinShininess) {
  PrincipledMaterial material(Color(1.0, 1.0, 1.0), /*metallic=*/0.0,
                               /*roughness=*/1.0);

  EXPECT_NEAR(material.shininess(), 1.0, tolerance);
}

TEST(PrincipledBrdfTest, MetallicMirrorProducesHighlightUnderPointLight) {
  PrincipledMaterial material(Color(0.7, 0.7, 0.7), /*metallic=*/1.0,
                               /*roughness=*/0.0);

  const Vector3D normal(0.0, 1.0, 0.0);
  const Vector3D incoming(0.0, -1.0, 0.0);
  const Vector3D outgoing(0.0, 1.0, 0.0);

  const Color brdf = material.brdf(incoming, outgoing, normal);

  EXPECT_NEAR(brdf.r, 0.7, tolerance);
  EXPECT_NEAR(brdf.g, 0.7, tolerance);
  EXPECT_NEAR(brdf.b, 0.7, tolerance);
}

TEST(PrincipledBrdfTest, FullRoughnessFlattensTheHighlight) {
  PrincipledMaterial material(Color(0.6, 0.6, 0.6), /*metallic=*/0.0,
                               /*roughness=*/1.0, /*ior=*/1.5,
                               /*alpha=*/1.0);

  const Vector3D normal(0.0, 1.0, 0.0);
  const Vector3D incoming(0.0, -1.0, 0.0);
  const double sinTheta = std::sqrt(0.75);
  const Vector3D outgoing(sinTheta, 0.5, 0.0);

  const Color brdf = material.brdf(incoming, outgoing, normal);

  const double expectedDiffuse = 0.6 / raytracer::math::constants::PI;
  const Vector3D halfVector =
      (-incoming + outgoing).normalize();
  const double alignment = std::max(0.0, normal.dot(halfVector));
  const double expectedSpecular = dielectricF0 * std::pow(alignment, 1.0);
  EXPECT_NEAR(brdf.r, expectedDiffuse + expectedSpecular, tolerance);
}

TEST(PrincipledBrdfTest, AlphaScalesDiffuseTerm) {
  PrincipledMaterial opaque(Color(0.6, 0.6, 0.6), /*metallic=*/0.0,
                             /*roughness=*/1.0, /*ior=*/1.5,
                             /*alpha=*/1.0);
  PrincipledMaterial halfTransparent(Color(0.6, 0.6, 0.6), /*metallic=*/0.0,
                                      /*roughness=*/1.0, /*ior=*/1.5,
                                      /*alpha=*/0.5);

  const Vector3D normal(0.0, 1.0, 0.0);
  const Vector3D incoming(0.0, -1.0, 0.0);
  const Vector3D outgoing(0.0, 1.0, 0.0);

  const Color opaqueBrdf = opaque.brdf(incoming, outgoing, normal);
  const Color halfBrdf = halfTransparent.brdf(incoming, outgoing, normal);

  const double opaqueDiffuse = 0.6 / raytracer::math::constants::PI;
  const double halfDiffuse = 0.3 / raytracer::math::constants::PI;
  EXPECT_NEAR(opaqueBrdf.r - dielectricF0, opaqueDiffuse, tolerance);
  EXPECT_NEAR(halfBrdf.r - dielectricF0, halfDiffuse, tolerance);
}
