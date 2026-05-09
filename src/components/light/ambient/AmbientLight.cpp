/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** AmbientLight
*/

#include "AmbientLight.hpp"

namespace raytracer::components::light::ambient {

Ambient::Ambient() = default;

Ambient::Ambient(const raytracer::math::Color& color, double intensity)
    : color(color), intensity(intensity) {}

raytracer::math::Color Ambient::illuminate(
    const raytracer::math::Vector3D& /*point*/,
    const raytracer::scene::Scene& /*scene*/) const {
  return color * intensity;
}

raytracer::math::Vector3D Ambient::getDirection(
    const raytracer::math::Vector3D& /*point*/) const {
  return {0.0, 0.0, 0.0};
}

double Ambient::getIntensity() const { return intensity; }

bool Ambient::isOccluded(const raytracer::math::Vector3D& /*point*/,
                         const raytracer::scene::Scene& /*scene*/) const {
  return false;
}

void Ambient::setColor(const raytracer::math::Color& newColor) {
  color = newColor;
}

void Ambient::setIntensity(double newIntensity) { intensity = newIntensity; }

}  // namespace raytracer::components::light::ambient

