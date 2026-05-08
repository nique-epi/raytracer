/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Glossy
*/

#include "Glossy.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Optics.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::components::material {

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

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" gsl::owner<IMaterial*> createMaterial(
    double fuzz, const raytracer::math::Color& albedo) {
  return new raytracer::components::material::Glossy(fuzz, albedo);
}

extern "C" void destroyMaterial(gsl::owner<IMaterial*> mat) { delete mat; }
