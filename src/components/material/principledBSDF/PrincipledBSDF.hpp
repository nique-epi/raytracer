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

inline constexpr double defaultPrincipledRefractionIndex = 1.5;

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

  /**
   * @brief Schlick F0 derived from @c baseColor and @c metallic.
   *
   * Returns a small dielectric reflectance (0.04) for non-metals and the
   * full @c baseColor for pure metals, linearly interpolated by
   * @c metallic. This is the constant base reflectance used in the
   * direct-lighting highlight; it does NOT incorporate the angular
   * Fresnel factor (kept simple for the MVP).
   *
   * @returns Linear RGB F0.
   */
  [[nodiscard]] math::Color specularAlbedo() const override;

  /**
   * @brief Phong-equivalent exponent derived from @c roughness.
   *
   * Maps @c roughness in [0, 1] to a shininess in [1, 256] via
   * @c (1 - roughness)^4 * 255 + 1. Mirror-like surfaces produce a
   * narrow highlight, rough surfaces produce a wide, almost-flat one.
   *
   * @returns Phong exponent applied to @c max(0, N·H) in @c brdf.
   */
  [[nodiscard]] double shininess() const override;

  /**
   * @brief Direct-lighting BRDF using Blinn-Phong half-vector + Schlick.
   *
   * @param [in] incomingDirection Unit direction from the light toward
   *                               the shaded point.
   * @param [in] outgoingDirection Unit direction from the shaded point
   *                               toward the camera.
   * @param [in] normal            Unit surface normal.
   * @returns Diffuse Lambert (weighted by @c (1-metallic)*alpha) plus
   *          Blinn-Phong specular (weighted by Schlick @c F0).
   */
  [[nodiscard]] math::Color brdf(
      const raytracer::math::Vector3D& incomingDirection,
      const raytracer::math::Vector3D& outgoingDirection,
      const raytracer::math::Vector3D& normal) const override;

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
