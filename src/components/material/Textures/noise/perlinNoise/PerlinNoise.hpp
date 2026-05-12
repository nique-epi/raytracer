/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PerlinNoise
*/

#pragma once

#include <array>
#include "utils/math/Vector3D.hpp"

namespace raytracer::materials::textures::utils {

class PerlinNoise {
 public:
  PerlinNoise();
  ~PerlinNoise() = default;

  PerlinNoise(const PerlinNoise&) = delete;
  PerlinNoise& operator=(const PerlinNoise&) = delete;
  PerlinNoise(PerlinNoise&&) = delete;
  PerlinNoise& operator=(PerlinNoise&&) = delete;

  [[nodiscard]] double noise(const math::Vector3D& point) const;
  [[nodiscard]] double turbulence(const math::Vector3D& point, int depth) const;

 private:
  static constexpr int PointCount = 256;

  std::array<math::Vector3D, PointCount> gradients_;
  std::array<int, PointCount> permX_;
  std::array<int, PointCount> permY_;
  std::array<int, PointCount> permZ_;

  static std::array<int, PointCount> generatePermutation();
};
}  // namespace raytracer::materials::textures::utils
