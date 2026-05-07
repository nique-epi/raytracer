/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** IBackground
*/

#pragma once

#include "utils/math/Color.hpp"

namespace raytracer::math {
class Ray;
}  // namespace raytracer::math

namespace raytracer::scene::background {

class IBackground {
 public:
  IBackground() = default;
  virtual ~IBackground() = default;

  IBackground(const IBackground&) = delete;
  IBackground& operator=(const IBackground&) = delete;
  IBackground(IBackground&&) = delete;
  IBackground& operator=(IBackground&&) = delete;

  [[nodiscard]] virtual raytracer::math::Color getColor(
      const raytracer::math::Ray& ray) const = 0;
};

}  // namespace raytracer::scene::background
