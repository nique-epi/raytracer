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
#include "utils/math/Ray.hpp"

namespace raytracer::math {
class HitRecord;
class Vector3D;
}  // namespace raytracer::math

/**
 * @brief Pure interface shared by all scene materials.
 *
 * A material describes how a surface interacts with light: it defines
 * how an incoming ray is scattered (or absorbed) and whether the surface
 * itself emits radiance.
 *
 * @note Prefer inheriting from AMaterial rather than IMaterial directly —
 *       AMaterial provides a sensible default for emitted() (returns black)
 *       so non-emissive materials do not need to override it.
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
   * @returns Emitted color. Non-emissive materials return black.
   */
  [[nodiscard]] virtual raytracer::math::Color emitted() const = 0;

  /**
   * @brief Lambertian albedo used to weight direct lighting.
   *
   * Direct-lighting contribution at a shaded point is evaluated by the
   * renderer as `brdf(L, V, N) * incomingRadiance * max(0, N · -L)`.
   * Materials whose response to direct light is purely specular or
   * refractive (mirrors, glass) return black so a `PointLight` does not
   * paint them as if they were Lambertian. Their lighting comes from the
   * scattered ray instead.
   *
   * @returns Linear RGB albedo. `Color(0, 0, 0)` for non-diffuse
   *          materials.
   */
  [[nodiscard]] virtual raytracer::math::Color diffuseAlbedo() const = 0;

  /**
   * @brief Specular reflectance constant (Phong @c ks).
   *
   * Used by @c brdf to weight the Phong specular highlight. Materials with
   * no highlight (pure Lambert) return @c Color(0, 0, 0) so the specular
   * term collapses to zero. @c PrincipledMaterial derives this from its
   * @c metallic / @c baseColor via Schlick @c F0.
   *
   * @returns Linear RGB specular reflectance. @c Color(0, 0, 0) disables
   *          the highlight.
   */
  [[nodiscard]] virtual raytracer::math::Color specularAlbedo() const = 0;

  /**
   * @brief Phong shininess exponent (@c alpha).
   *
   * Controls highlight width: larger values produce smaller, sharper
   * highlights. Must be strictly positive. Materials with no highlight
   * return @c 1.0 (the value is irrelevant when @c specularAlbedo is
   * black).
   *
   * @returns Phong exponent applied to @c max(0, R·V) in @c AMaterial::brdf
   *          and to @c max(0, N·H) in @c PrincipledMaterial::brdf.
   */
  [[nodiscard]] virtual double shininess() const = 0;

  /**
   * @brief Evaluate the surface BRDF (bidirectional reflectance distribution)
   *        at a shading point.
   *
   * The integrator multiplies the returned value by `incomingRadiance`
   * and the geometry term `max(0, N · -incomingDirection)` to obtain the
   * outgoing direct-lighting contribution along @p outgoingDirection.
   *
   * @param [in] incomingDirection Unit direction from the light toward the
   *                               shaded point (i.e. ray direction the
   *                               light would travel).
   * @param [in] outgoingDirection Unit direction from the shaded point
   *                               toward the camera.
   * @param [in] normal            Unit surface normal at the shaded point.
   * @returns BRDF value per steradian, already normalised (Lambert returns
   *          `albedo / PI`).
   */
  [[nodiscard]] virtual raytracer::math::Color brdf(
      const raytracer::math::Vector3D& incomingDirection,
      const raytracer::math::Vector3D& outgoingDirection,
      const raytracer::math::Vector3D& normal) const = 0;
};

#endif  // MATERIAL_IMATERIAL_HPP_
