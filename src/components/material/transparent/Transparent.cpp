/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Transparent
*/

#include "Transparent.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace {
constexpr double transparentRayEpsilon = 1e-4;
}  // namespace

namespace raytracer::components::material {

Transparent::Transparent(const raytracer::math::Color& tint) : tint_(tint) {}

bool Transparent::scatter(const raytracer::math::Ray& in,
                          const raytracer::math::HitRecord& rec,
                          raytracer::math::Color& attenuation,
                          raytracer::math::Ray& scattered) const {
  const raytracer::math::Vector3D direction = in.getDirection().normalize();
  scattered = raytracer::math::Ray(
      rec.point + (direction * transparentRayEpsilon), direction);
  attenuation = tint_;
  return true;
}

const raytracer::math::Color& Transparent::tint() const { return tint_; }

}  // namespace raytracer::components::material
