/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Transparent
*/

#pragma once

#include "components/material/abstract/AMaterial.hpp"
#include "utils/math/Color.hpp"

namespace raytracer::components::material {

/**
 * @brief Pass-through transparent material — no refraction, no reflection.
 *
 * Models a thin transparent medium (cellophane, plastic film, perfectly
 * matched-index glass): the incoming ray crosses the surface unaffected,
 * carrying an optional color @c tint. Unlike @c Glass, there is no Snell
 * deviation and no Fresnel mix; unlike @c PrincipledMaterial's @c alpha,
 * the effect is fully deterministic.
 *
 * The material has no direct-lighting response (@c diffuseAlbedo returns
 * black through @c AMaterial), so shadow rays from punctual lights do
 * not contribute. The only radiance carried back is the scattered ray's
 * contribution attenuated by @p tint.
 */
class Transparent : public AMaterial {
 public:
  /**
   * @brief Construct a transparent material.
   *
   * @param [in] tint Color filter applied to the ray crossing the
   *                  surface. @c Color(1, 1, 1) is fully neutral
   *                  (perfectly clear).
   */
  explicit Transparent(const raytracer::math::Color& tint
                       = raytracer::math::Color(1.0, 1.0, 1.0));

  ~Transparent() override = default;

  Transparent(const Transparent&) = delete;
  Transparent& operator=(const Transparent&) = delete;
  Transparent(Transparent&&) = delete;
  Transparent& operator=(Transparent&&) = delete;

  /**
   * @brief Continue the incoming ray straight through the surface.
   *
   * @param [in]  in          Incoming ray.
   * @param [in]  rec         Intersection record (provides @c point).
   * @param [out] attenuation Filled with @c tint_.
   * @param [out] scattered   Ray with the same direction as @p in,
   *                          offset past the surface to avoid
   *                          self-intersection.
   * @returns @c true always — a transparent surface never absorbs.
   */
  bool scatter(const raytracer::math::Ray& in,
               const raytracer::math::HitRecord& rec,
               raytracer::math::Color& attenuation,
               raytracer::math::Ray& scattered) const override;

  /**
   * @brief Access the configured tint color.
   *
   * @returns The color filter applied to rays crossing the surface.
   */
  [[nodiscard]] const raytracer::math::Color& tint() const;

 private:
  raytracer::math::Color tint_;
};

}  // namespace raytracer::components::material
