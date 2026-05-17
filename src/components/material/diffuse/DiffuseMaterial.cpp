/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** DiffuseMaterial
*/

#include "DiffuseMaterial.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::components::material {

DiffuseMaterial::DiffuseMaterial(const raytracer::math::Color& albedo,
                                  const raytracer::math::Color& specularAlbedo,
                                  double shininess)
    : AMaterial(specularAlbedo, shininess), albedo_(albedo) {}

bool DiffuseMaterial::scatter(const raytracer::math::Ray& /*in*/,
                              const raytracer::math::HitRecord& rec,
                              raytracer::math::Color& attenuation,
                              raytracer::math::Ray& scattered) const {
  raytracer::math::Vector3D scatterDir =
      rec.normal + raytracer::math::Vector3D::randomInUnitSphere().normalize();
  if (scatterDir.isNearZero()) {
    scatterDir = rec.normal;
  }
  scattered = raytracer::math::Ray(rec.point, scatterDir);
  attenuation = albedo_;
  return true;
}

raytracer::math::Color DiffuseMaterial::diffuseAlbedo() const {
  return albedo_;
}

}  // namespace raytracer::components::material
