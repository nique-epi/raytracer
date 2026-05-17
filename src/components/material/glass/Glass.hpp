/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Glass
*/

#pragma once

#include "components/material/abstract/AMaterial.hpp"

namespace raytracer::components::material {

class Glass : public AMaterial {
 public:
  Glass() = delete;

  /**
   * @brief Construct a dielectric (glass-like) material.
   *
   * @param [in] refractionIndex Snell's law index of refraction.
   * @param [in] tint            Color applied to the scattered ray.
   * @param [in] specularAlbedo  Phong @c ks for the direct-lighting
   *                             highlight (sharp glare on the glass
   *                             facing the light). @c Color(0, 0, 0)
   *                             disables it.
   * @param [in] shininess       Phong exponent @c alpha. Strictly
   *                             positive.
   */
  explicit Glass(double refractionIndex,
                 const math::Color& tint = math::Color(1.0, 1.0, 1.0),
                 const raytracer::math::Color& specularAlbedo
                     = raytracer::math::Color(0.0, 0.0, 0.0),
                 double shininess = 1.0);
  ~Glass() override = default;

  Glass(const Glass&) = delete;
  Glass& operator=(const Glass&) = delete;
  Glass(Glass&&) = delete;
  Glass& operator=(Glass&&) = delete;

  bool scatter(const raytracer::math::Ray& in,
               const raytracer::math::HitRecord& rec,
               raytracer::math::Color& attenuation,
               raytracer::math::Ray& scattered) const override;

 private:
  double refractionIndex_;
  math::Color tint_;
};
}  // namespace raytracer::components::material
