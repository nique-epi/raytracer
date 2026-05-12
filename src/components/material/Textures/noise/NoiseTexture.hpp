/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** NoiseTexture
*/

#pragma once

#include "components/material/Textures/ITexture.hpp"
#include "perlinNoise/PerlinNoise.hpp"

namespace raytracer::materials::textures {

class NoiseTexture : public ITexture {
 public:
  NoiseTexture() = default;
  ~NoiseTexture() override = default;

  NoiseTexture(const NoiseTexture&) = delete;
  NoiseTexture& operator=(const NoiseTexture&) = delete;
  NoiseTexture(NoiseTexture&&) = delete;
  NoiseTexture& operator=(NoiseTexture&&) = delete;

  [[nodiscard]] math::Color sample(double u, double v,
                                   const math::Vector3D& point) const override;

 private:
  utils::PerlinNoise perlin_;
  double scale_ = 1.0;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  int turbulence_depth_ = 7;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  static constexpr double turbulenceAmplitude = 10.0;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  static constexpr double colorScale = 0.5;
};  // namespace raytracer::class
}  // namespace raytracer::materials::textures
