/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Glossy
*/

#pragma once

#include "components/material/abstract/AMaterial.hpp"
#include "utils/math/Color.hpp"

namespace raytracer::components::material {

class Glossy : public AMaterial {
 public:
  Glossy() = default;
  ~Glossy() override = default;

  /**
   * @brief Construct a glossy (rough-mirror) material.
   *
   * @param [in] fuzz           Roughness factor in [0, 1] (clamped). 0 =
   *                            perfect mirror, 1 = uniformly scattered.
   * @param [in] albedo         Surface tint applied to the scattered ray.
   * @param [in] specularAlbedo Phong @c ks for the direct-lighting
   *                            highlight. @c Color(0, 0, 0) disables it.
   * @param [in] shininess      Phong exponent @c alpha. Strictly positive.
   */
  Glossy(double fuzz, const math::Color& albedo,
         const raytracer::math::Color& specularAlbedo
             = raytracer::math::Color(0.0, 0.0, 0.0),
         double shininess = 1.0);

  Glossy(const Glossy&) = delete;
  Glossy& operator=(const Glossy&) = delete;
  Glossy(Glossy&&) = delete;
  Glossy& operator=(Glossy&&) = delete;

  bool scatter(const raytracer::math::Ray& in,
               const raytracer::math::HitRecord& rec,
               raytracer::math::Color& attenuation,
               raytracer::math::Ray& scattered) const override;

 private:
  double fuzz{0.0};
  math::Color albedo{1.0, 1.0, 1.0};
};
}  // namespace raytracer::components::material
