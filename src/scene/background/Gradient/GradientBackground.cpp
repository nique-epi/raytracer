/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** GradientBackground
*/

#include "GradientBackground.hpp"
#include "utils/math/Ray.hpp"

namespace {
constexpr double HALF = 0.5;
}  // namespace

namespace raytracer::scene::background {

[[nodiscard]] raytracer::math::Color GradientBackground::getColor(
    const raytracer::math::Ray& ray) const {
  double t = HALF * (ray.getDirection().y + 1.0);
  return (1.0 - t) * topColor_ + t * bottomColor_;
}
}  // namespace raytracer::scene::background
