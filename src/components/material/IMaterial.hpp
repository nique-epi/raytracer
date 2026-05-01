/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** IMaterial
*/

/**
 * @file IMaterial.hpp
 * @brief Contract shared by all scene materials.
 */

#ifndef MATERIAL_IMATERIAL_HPP_
#define MATERIAL_IMATERIAL_HPP_

#include "utils/math/Color.hpp"

namespace raytracer::math {
class Ray;
class HitRecord;
}  // namespace raytracer::math

/**
 * @brief Contract shared by all scene materials.
 *
 * A material describes how a surface interacts with light: it defines
 * how an incoming ray is scattered (or absorbed) and whether the surface
 * itself emits radiance.
 *
 * Implementors must override scatter(). emitted() may be left at its default
 * (returns black) for non-emissive materials.
 */
class IMaterial {
 public:
  IMaterial() = default;
  virtual ~IMaterial() = default;

  IMaterial(const IMaterial&) = delete;
  IMaterial& operator=(const IMaterial&) = delete;
  IMaterial(IMaterial&&) = delete;
  IMaterial& operator=(IMaterial&&) = delete;

  /**
   * @brief Compute the scattered ray and attenuation for an incoming ray.
   *
   * @param [in]  in          Incoming ray that hit the surface.
   * @param [in]  rec         Intersection record for the hit point.
   * @param [out] attenuation Color factor applied to the scattered ray's
   *                          contribution.
   * @param [out] scattered   The outgoing ray after scattering.
   * @returns true  if the ray is scattered (i.e. not fully absorbed).
   * @returns false if the ray is absorbed; @p attenuation and @p scattered
   *                are left unmodified.
   */
  virtual bool scatter(const raytracer::math::Ray& in,
                       const raytracer::math::HitRecord& rec,
                       raytracer::math::Color& attenuation,
                       raytracer::math::Ray& scattered) const = 0;

  /**
   * @brief Return the radiance emitted by this material.
   *
   * The default implementation returns black (no emission), making this
   * method optional for non-emissive materials such as Lambertian or metal.
   *
   * @returns Emitted color; defaults to Color(0, 0, 0).
   */
  virtual raytracer::math::Color emitted() const;
};

#endif  // MATERIAL_IMATERIAL_HPP_
