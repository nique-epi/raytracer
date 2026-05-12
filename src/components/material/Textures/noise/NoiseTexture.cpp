/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** NoiseTexture
*/

#include "NoiseTexture.hpp"
#include <cmath>

namespace raytracer::materials::textures {
[[nodiscard]] math::Color NoiseTexture::sample(
    double /*u*/, double /*v*/, const math::Vector3D& point) const {
  return math::Color(1, 1, 1) * colorScale *
         (1.0 +
          std::sin((scale_ * point.z) +
                   (turbulenceAmplitude * perlin_.turbulence(point, turbulence_depth_))));
}
}  // namespace raytracer::materials::textures
