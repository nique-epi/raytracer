/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Directional
*/

#include "Directional.hpp"
#include <limits>
#include "scene/Scene.hpp"
#include "utils/math/Constants.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"

namespace raytracer::components::light::directional {

Directional::Directional() = default;

Directional::Directional(const raytracer::math::Vector3D& direction,
                         const raytracer::math::Color& color, double intensity)
    : direction(direction.normalize()), color(color), intensity(intensity) {}

raytracer::math::Color Directional::illuminate(
    const raytracer::math::Vector3D& point,
    const raytracer::scene::Scene& scene) const {
  if (isOccluded(point, scene)) {
    return {0.0, 0.0, 0.0};
  }
  return color * intensity;
}

raytracer::math::Vector3D Directional::getDirection(
    const raytracer::math::Vector3D& /*point*/) const {
  return direction;
}

double Directional::getIntensity() const { return intensity; }

bool Directional::isOccluded(const raytracer::math::Vector3D& point,
                             const raytracer::scene::Scene& scene) const {
  const raytracer::math::Ray shadowRay(point, -direction);
  raytracer::math::HitRecord rec;
  return scene.hit(shadowRay, raytracer::math::constants::epsilon,
                   std::numeric_limits<double>::infinity(), rec);
}

void Directional::setDirection(const raytracer::math::Vector3D& newDirection) {
  direction = newDirection.normalize();
}

void Directional::setColor(const raytracer::math::Color& newColor) {
  color = newColor;
}

void Directional::setIntensity(double newIntensity) {
  intensity = newIntensity;
}

}  // namespace raytracer::components::light::directional

