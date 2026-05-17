/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Hemisphere sampling helpers (cosine-weighted)
*/

#include "rendering/helper/HemisphereSampler.hpp"

#include <algorithm>
#include <cmath>
#include <random>

#include "utils/math/Constants.hpp"

namespace raytracer::rendering::helper {

namespace {

std::mt19937& threadLocalEngine() {
  thread_local std::mt19937 engine{std::random_device{}()};
  return engine;
}

math::Vector3D buildTangent(const math::Vector3D& normal) {
  if (std::abs(normal.x) > std::abs(normal.z)) {
    const double length = std::sqrt((normal.x * normal.x) +
                                    (normal.y * normal.y));
    return {-normal.y / length, normal.x / length, 0.0};
  }
  const double length = std::sqrt((normal.y * normal.y) +
                                  (normal.z * normal.z));
  return {0.0, -normal.z / length, normal.y / length};
}

}  // namespace

math::Vector3D sampleCosineHemisphere(const math::Vector3D& normal) {
  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  auto& engine = threadLocalEngine();
  const double u1 = distribution(engine);
  const double u2 = distribution(engine);

  const double radius = std::sqrt(u1);
  const double azimuth = math::constants::DOUBLE * math::constants::PI * u2;
  const double localX = radius * std::cos(azimuth);
  const double localY = radius * std::sin(azimuth);
  const double localZ = std::sqrt(std::max(0.0, 1.0 - u1));

  const math::Vector3D tangent = buildTangent(normal);
  const math::Vector3D bitangent = normal.cross(tangent);

  return ((tangent * localX) + (bitangent * localY) + (normal * localZ))
      .normalize();
}

void seedHemisphereSampler(std::uint64_t seed) {
  threadLocalEngine().seed(seed);
}

}  // namespace raytracer::rendering::helper
