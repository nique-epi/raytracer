/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** HemisphereSampler unit tests
*/

#include <gtest/gtest.h>
#include "rendering/helper/HemisphereSampler.hpp"
#include "utils/math/Vector3D.hpp"

using raytracer::math::Vector3D;
using raytracer::rendering::helper::sampleCosineHemisphere;
using raytracer::rendering::helper::seedHemisphereSampler;

namespace {

constexpr int sampleBatchSize = 500;
constexpr double unitLengthTolerance = 1e-9;
constexpr double hemisphereTolerance = -1e-12;

}  // namespace

TEST(HemisphereSamplerTest, SamplesLieAboveTheHemispherePlaneForCanonicalNormal) {
  seedHemisphereSampler(42);
  const Vector3D normal(0.0, 1.0, 0.0);

  for (int index = 0; index < sampleBatchSize; ++index) {
    const Vector3D sample = sampleCosineHemisphere(normal);
    EXPECT_GE(sample.dot(normal), hemisphereTolerance);
  }
}

TEST(HemisphereSamplerTest, SamplesLieAboveTheHemispherePlaneForObliqueNormal) {
  seedHemisphereSampler(7);
  const Vector3D normal = Vector3D(0.3, 0.7, -0.4).normalize();

  for (int index = 0; index < sampleBatchSize; ++index) {
    const Vector3D sample = sampleCosineHemisphere(normal);
    EXPECT_GE(sample.dot(normal), hemisphereTolerance);
  }
}

TEST(HemisphereSamplerTest, SamplesAreUnitLength) {
  seedHemisphereSampler(123);
  const Vector3D normal = Vector3D(0.0, 0.0, 1.0);

  for (int index = 0; index < sampleBatchSize; ++index) {
    const Vector3D sample = sampleCosineHemisphere(normal);
    EXPECT_NEAR(sample.length(), 1.0, unitLengthTolerance);
  }
}

TEST(HemisphereSamplerTest, SamplesAreReproducibleWithSameSeed) {
  const Vector3D normal(0.0, 1.0, 0.0);

  seedHemisphereSampler(2026);
  const Vector3D firstA = sampleCosineHemisphere(normal);
  const Vector3D secondA = sampleCosineHemisphere(normal);

  seedHemisphereSampler(2026);
  const Vector3D firstB = sampleCosineHemisphere(normal);
  const Vector3D secondB = sampleCosineHemisphere(normal);

  EXPECT_DOUBLE_EQ(firstA.x, firstB.x);
  EXPECT_DOUBLE_EQ(firstA.y, firstB.y);
  EXPECT_DOUBLE_EQ(firstA.z, firstB.z);
  EXPECT_DOUBLE_EQ(secondA.x, secondB.x);
}
