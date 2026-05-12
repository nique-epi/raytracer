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
  // Degenerate case: the shading point coincides with the light's
  // position. Physically the radiance diverges (1 / 0), so we skip the
  // contribution rather than emit infinite or NaN values that would
  // poison the renderer's accumulator.
  if (distanceSquared <= raytracer::math::constants::epsilon) {
    return {0.0, 0.0, 0.0};
  }
  if (isOccluded(point, scene)) {
    return {0.0, 0.0, 0.0};
  }
  return color * intensity / distanceSquared;
}

raytracer::math::Vector3D PointLight::getDirection(
    const raytracer::math::Vector3D& point) const {
  const raytracer::math::Vector3D fromLight = point - position;
  // Degenerate case: the shading point sits exactly at the light
  // position. The propagation direction is undefined, so we return the
  // zero-vector sentinel documented by ILight. Returning a normalised
  // vector here would throw because Vector3D::normalize() rejects a
  // zero-length input.
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
  // Degenerate case: the shading point is at the light position. There
  // is no segment to test for occlusion, so we report "not occluded" —
  // illuminate() handles the zero-distance case separately and skips
  // the contribution anyway, so this is just a safety fallback.
  if (distance <= raytracer::math::constants::epsilon) {
    return false;
  }
  const raytracer::math::Ray shadowRay(point, toLight / distance);
  raytracer::math::HitRecord rec;
  return scene.hit(shadowRay, raytracer::math::constants::epsilon, distance,
                   rec);
}

}  // namespace raytracer::components::light::point
