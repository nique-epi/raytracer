/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PrincipledMaterial
*/

#pragma once

#include "components/material/abstract/AMaterial.hpp"
#include "utils/math/Color.hpp"

namespace raytracer::components::material {

inline constexpr double defaultPrincipledRefractionIndex = 1.45;

/**
 * @brief Simplified Principled BSDF material.
 *
 * Combines diffuse, metallic, and dielectric properties into a single material.
 */
class PrincipledMaterial : public AMaterial {
 public:
  PrincipledMaterial(const math::Color& baseColor, double metallic,
                     double roughness,
                     double ior = defaultPrincipledRefractionIndex,
                     double alpha = 1.0);

  PrincipledMaterial(const PrincipledMaterial&) = delete;
  PrincipledMaterial& operator=(const PrincipledMaterial&) = delete;
  PrincipledMaterial(PrincipledMaterial&&) = delete;
  PrincipledMaterial& operator=(PrincipledMaterial&&) = delete;
  ~PrincipledMaterial() override = default;

  bool scatter(const raytracer::math::Ray& in,
               const raytracer::math::HitRecord& rec,
               raytracer::math::Color& attenuation,
               raytracer::math::Ray& scattered) const override;

  [[nodiscard]] math::Color diffuseAlbedo() const override;

 private:
  bool scatterMetallic(const raytracer::math::Ray& incomingRay,
                       const raytracer::math::HitRecord& hitRecord,
                       raytracer::math::Color& attenuation,
                       raytracer::math::Ray& scatteredRay) const;

  bool scatterSpecularReflection(const raytracer::math::Ray& incomingRay,
                                 const raytracer::math::HitRecord& hitRecord,
                                 raytracer::math::Color& attenuation,
                                 raytracer::math::Ray& scatteredRay) const;

  bool scatterDiffuseReflection(const raytracer::math::HitRecord& hitRecord,
                                raytracer::math::Color& attenuation,
                                raytracer::math::Ray& scatteredRay) const;

  math::Color baseColor_;
  double metallic_;
  double roughness_;
  double ior_;
  double alpha_;
};

}  // namespace raytracer::components::material
