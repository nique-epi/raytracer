/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Glossy
*/

#include "Glossy.hpp"
#include <algorithm>
#include "utils/math/HitRecord.hpp"
#include "utils/math/Optics.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::components::material {

Glossy::Glossy(double fuzz, const math::Color& albedo,
               const raytracer::math::Color& specularAlbedo, double shininess)
    : AMaterial(specularAlbedo, shininess),
      fuzz(std::clamp(fuzz, 0.0, 1.0)),
      albedo(albedo) {}

bool Glossy::scatter(const raytracer::math::Ray& in,
                     const raytracer::math::HitRecord& rec,
                     raytracer::math::Color& attenuation,
                     raytracer::math::Ray& scattered) const {
  raytracer::math::Vector3D reflected =
      math::Optics::reflect(in.getDirection().normalize(), rec.normal);
  raytracer::math::Vector3D scatteredDir =
      reflected + fuzz * raytracer::math::Vector3D::randomInUnitSphere();
  if (scatteredDir.dot(rec.normal) < 0) {
    return false;
  }
  scattered = raytracer::math::Ray(rec.point, scatteredDir);
  attenuation = albedo;
  return true;
}
}  // namespace raytracer::components::material
