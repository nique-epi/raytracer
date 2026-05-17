/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TexturedMaterial
*/

#include "TexturedMaterial.hpp"
#include <memory>
#include <utility>
#include "components/material/Textures/ITexture.hpp"  // NOLINT(misc-include-cleaner)
#include "utils/math/Color.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::components::material {

// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init,hicpp-member-init)
TexturedMaterial::TexturedMaterial(
    std::shared_ptr<raytracer::materials::ITexture> texture,
    const raytracer::math::Color& albedo,
    const raytracer::math::Color& specularAlbedo, double shininess)
    : AMaterial(specularAlbedo, shininess),
      texture_(std::move(texture)),
      albedo_(albedo) {}

bool TexturedMaterial::scatter(const raytracer::math::Ray& /*in*/,
                               const raytracer::math::HitRecord& rec,
                               raytracer::math::Color& attenuation,
                               raytracer::math::Ray& scattered) const {
  raytracer::math::Vector3D scatterDir =
      rec.normal + raytracer::math::Vector3D::randomInUnitSphere().normalize();
  if (scatterDir.isNearZero()) {
    scatterDir = rec.normal;
  }
  scattered = raytracer::math::Ray(rec.point, scatterDir);
  attenuation = texture_->sample(rec.u, rec.v, rec.point) * albedo_;
  return true;
}

}  // namespace raytracer::components::material
