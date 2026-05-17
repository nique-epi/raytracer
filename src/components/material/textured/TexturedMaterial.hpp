/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TexturedMaterial
*/

#pragma once

#include <memory>
#include "components/material/Textures/ITexture.hpp"
#include "components/material/abstract/AMaterial.hpp"
#include "utils/math/Color.hpp"

namespace raytracer::components::material {

/**
 * @brief Lambertian diffuse material driven by an ITexture.
 *
 * Scatters incoming rays like DiffuseMaterial, but the attenuation is
 * computed as texture->sample(u, v, point) * albedo instead of a flat color.
 */
class TexturedMaterial : public AMaterial {
 public:
  /**
   * @brief Construct a textured material with optional Phong highlight.
   * @param [in] texture        Texture used to modulate the surface color.
   * @param [in] albedo         Base color multiplied by the texture sample.
   * @param [in] specularAlbedo Phong @c ks. @c Color(0, 0, 0) disables the
   *                            highlight (default).
   * @param [in] shininess      Phong exponent @c alpha. Strictly positive.
   */
  TexturedMaterial(std::shared_ptr<raytracer::materials::ITexture> texture,
                   const raytracer::math::Color& albedo,
                   const raytracer::math::Color& specularAlbedo
                       = raytracer::math::Color(0.0, 0.0, 0.0),
                   double shininess = 1.0);

  TexturedMaterial(const TexturedMaterial&) = delete;
  TexturedMaterial& operator=(const TexturedMaterial&) = delete;
  TexturedMaterial(TexturedMaterial&&) = delete;
  TexturedMaterial& operator=(TexturedMaterial&&) = delete;
  ~TexturedMaterial() override = default;

  /**
   * @brief Scatter the incoming ray with texture-modulated attenuation.
   * @param [in]  in          Incoming ray.
   * @param [in]  rec         Intersection record (provides u, v, point).
   * @param [out] attenuation texture->sample(u, v, point) * albedo.
   * @param [out] scattered   Diffuse scattered ray.
   * @returns true always (Lambertian never fully absorbs).
   */
  bool scatter(const raytracer::math::Ray& in,
               const raytracer::math::HitRecord& rec,
               raytracer::math::Color& attenuation,
               raytracer::math::Ray& scattered) const override;

 private:
  std::shared_ptr<raytracer::materials::ITexture> texture_;
  raytracer::math::Color albedo_;
};

}  // namespace raytracer::components::material
