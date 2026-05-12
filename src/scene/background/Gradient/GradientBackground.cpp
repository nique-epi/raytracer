/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** GradientBackground
*/

#include "GradientBackground.hpp"
#include "utils/math/Constants.hpp"
#include "utils/math/Ray.hpp"

namespace raytracer::scene::background {

[[nodiscard]] raytracer::math::Color GradientBackground::getColor(
    const raytracer::math::Ray& ray) const {
  double t = raytracer::math::constants::HALF *
             (ray.getDirection().normalize().y + 1.0);
  return (1.0 - t) * topColor_ + t * bottomColor_;
}
}  // namespace raytracer::scene::background
