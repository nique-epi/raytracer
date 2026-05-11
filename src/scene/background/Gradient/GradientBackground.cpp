/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** GradientBackground
*/

#include "GradientBackground.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::scene::background {

GradientBackground::GradientBackground(const raytracer::math::Color& bottomColor,
                                       const raytracer::math::Color& topColor)
    : _bottomColor(bottomColor), _topColor(topColor) {}

raytracer::math::Color GradientBackground::getColor(
    const raytracer::math::Ray& ray) const {
  const raytracer::math::Vector3D dir = ray.getDirection().normalize();
  constexpr double half = 0.5;
  const double t = half * (dir.y + 1.0);
  return (1.0 - t) * _bottomColor + t * _topColor;
}

}  // namespace raytracer::scene::background
