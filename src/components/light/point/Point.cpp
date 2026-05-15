/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Point
*/

#include "Point.hpp"
#include "scene/Scene.hpp"
#include "utils/math/Constants.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"

namespace raytracer::components::light::point {

PointLight::PointLight() = default;

PointLight::PointLight(const raytracer::math::Vector3D& position,
                       const raytracer::math::Color& color, double intensity)
    : position(position), color(color), intensity(intensity) {}

raytracer::math::Color PointLight::illuminate(
    const raytracer::math::Vector3D& point,
    const raytracer::scene::Scene& scene) const {
  const double distanceSquared = (position - point).lengthSquared();
  if (distanceSquared <= raytracer::math::constants::epsilon) {
    return {0.0, 0.0, 0.0};
  }
  if (isOccluded(point, scene)) {
    return {0.0, 0.0, 0.0};
  }
  return (color * intensity) /
         (4.0 * raytracer::math::constants::pi * distanceSquared);
}

raytracer::math::Vector3D PointLight::getDirection(
    const raytracer::math::Vector3D& point) const {
  const raytracer::math::Vector3D fromLight = point - position;
  if (fromLight.lengthSquared() <= raytracer::math::constants::epsilon) {
    return {0.0, 0.0, 0.0};
  }
  return fromLight.normalize();
}

double PointLight::getIntensity() const { return intensity; }

bool PointLight::isOccluded(const raytracer::math::Vector3D& point,
                            const raytracer::scene::Scene& scene) const {
  const raytracer::math::Vector3D toLight = position - point;
  const double distance = toLight.length();
  if (distance <= raytracer::math::constants::epsilon) {
    return false;
  }
  const raytracer::math::Ray shadowRay(point, toLight / distance);
  raytracer::math::HitRecord rec;
  return scene.hit(shadowRay, raytracer::math::constants::epsilon, distance,
                   rec);
}

}  // namespace raytracer::components::light::point
