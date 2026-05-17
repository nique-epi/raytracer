/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** DiffuseMaterial
*/

#pragma once

#include "components/material/abstract/AMaterial.hpp"
#include "utils/math/Color.hpp"

namespace raytracer::components::material {

/**
 * @brief Lambertian diffuse material.
 *
 * Scatters incoming rays uniformly in the hemisphere above the surface
 * normal. The attenuation is always equal to the albedo.
 * Inherits the default non-emissive emitted() from AMaterial.
 */
class DiffuseMaterial : public AMaterial {
 public:
  /**
   * @brief Construct a Lambertian material with optional Phong highlight.
   *
   * @param [in] albedo         Lambertian diffuse albedo (kd).
   * @param [in] specularAlbedo Phong @c ks. @c Color(0, 0, 0) disables the
   *                            highlight (default).
   * @param [in] shininess      Phong exponent @c alpha. Strictly positive.
   */
  explicit DiffuseMaterial(
      const raytracer::math::Color& albedo,
      const raytracer::math::Color& specularAlbedo
          = raytracer::math::Color(0.0, 0.0, 0.0),
      double shininess = 1.0);

  DiffuseMaterial(const DiffuseMaterial&) = delete;
  DiffuseMaterial& operator=(const DiffuseMaterial&) = delete;
  DiffuseMaterial(DiffuseMaterial&&) = delete;
  DiffuseMaterial& operator=(DiffuseMaterial&&) = delete;
  ~DiffuseMaterial() override = default;

  bool scatter(const raytracer::math::Ray& in,
               const raytracer::math::HitRecord& rec,
               raytracer::math::Color& attenuation,
               raytracer::math::Ray& scattered) const override;

  [[nodiscard]] raytracer::math::Color diffuseAlbedo() const override;

 private:
  raytracer::math::Color albedo_;
};

}  // namespace raytracer::components::material
