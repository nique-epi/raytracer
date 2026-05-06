/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ILight
*/

#pragma once

#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::scene {
class Scene;
}  // namespace raytracer::scene

/**
 * @brief Contract shared by every light source in the scene.
 *
 * Lights answer the question "how much light reaches this surface
 * point, and from what direction?" They do not apply the surface BRDF
 * (Lambert, Phong, …) themselves — that is the integrator's job. The
 * split keeps lights simple and lets the integrator combine multiple
 * lights with the surface material.
 *
 * Reference integrator pseudo-code (Whitted-style direct lighting):
 * @code
 *   Color shade(Point P, Vector N, Material mat, Scene scn, Lights L) {
 *     Color out{0, 0, 0};
 *     for (auto& light : L) {
 *       Color radiance = light.illuminate(P, scn);   // already 0 if shadowed
 *       Vector3D dir = light.getDirection(P);
 *       if (dir.lengthSquared() == 0.0) {
 *         // Ambient sentinel: no geometric attenuation.
 *         out += radiance * mat.ambient;
 *       } else {
 *         double cosTheta = std::max(0.0, N.dot(-dir));
 *         out += radiance * mat.diffuse * cosTheta;
 *       }
 *     }
 *     return out;
 *   }
 * @endcode
 *
 * Conventions used throughout this interface:
 *  - All directions are unit vectors unless noted otherwise.
 *  - Colours are linear RGB; channels are non-negative reals, not
 *    bounded above (HDR is allowed).
 *  - The integrator owns surface BRDF, normal-cosine factor, and
 *    shadow-ray epsilon offsetting (`point + N * epsilon`).
 *  - The light owns its own visibility test (`isOccluded`) and its
 *    own emission (`illuminate`).
 */
class ILight {
 public:
  virtual ~ILight() = default;

  /**
   * @brief Radiance arriving at @p point from this light.
   *
   * Implementations return the light's emitted colour times its
   * intensity, **already attenuated by occlusion** (i.e. black if the
   * light is blocked from @p point). The cosine factor between the
   * surface normal and the light direction is **not** included — the
   * integrator applies it after combining with the BRDF.
   *
   * Distance falloff (relevant for `Point` / `Spot` lights) is part
   * of "radiance arriving" and is therefore the light's
   * responsibility, not the integrator's.
   *
   * @param [in] point Surface point being shaded, in world space.
   * @param [in] scene Scene used for the visibility test inside
   *                   @ref isOccluded. Implementations are free to
   *                   skip occlusion if it is meaningless for them
   *                   (e.g. ambient lights).
   * @returns Linear RGB radiance reaching @p point. `(0, 0, 0)` when
   *          the light is fully shadowed.
   */
  [[nodiscard]] virtual raytracer::math::Color illuminate(
      const raytracer::math::Vector3D& point,
      const raytracer::scene::Scene& scene) const = 0;

  /**
   * @brief Propagation direction of the light at @p point.
   *
   * Returns the unit vector along which photons travel **from the
   * source to** @p point. Equivalently, the integrator computes
   * "direction toward light" as `-getDirection(point)` when applying
   * Lambert's law: `dot(N, -getDirection(P))`.
   *
   * Per-light-type behaviour:
   *  - `Directional`: constant unit vector, independent of @p point.
   *  - `Point` / `Spot`: `(point - lightPosition).normalize()`,
   *    varies with @p point.
   *  - `Ambient`: returns the **zero vector** `(0, 0, 0)` as a
   *    sentinel meaning "no defined direction". Integrators must
   *    test `lengthSquared() == 0` and skip the cosine factor in
   *    that case.
   *
   * @param [in] point Surface point being shaded, in world space.
   *                   Ignored by lights with a constant direction.
   * @returns Unit-length propagation direction, or zero vector for
   *          ambient/omni-directional contributions.
   */
  [[nodiscard]] virtual raytracer::math::Vector3D getDirection(
      const raytracer::math::Vector3D& point) const = 0;

  /**
   * @brief Scalar intensity multiplier of the light.
   *
   * Dimensionless multiplier applied to the light's colour. `1.0`
   * is the nominal full-strength value; HDR scenes may exceed it.
   *
   * Note that @ref illuminate already returns `colour * intensity`
   * (modulo occlusion). `getIntensity()` is exposed separately so
   * algorithms that need the multiplier in isolation — importance
   * sampling, multi-light weighting, exposure tweaks — do not have
   * to back-derive it from the radiance.
   *
   * @returns Non-negative scalar.
   */
  [[nodiscard]] virtual double getIntensity() const = 0;

  /**
   * @brief Whether @p point is in shadow with respect to this light.
   *
   * Implementations cast a shadow ray from @p point toward the light
   * source and return `true` if any geometry from @p scene blocks
   * the ray within the light's reachable range:
   *  - `Directional`: ray direction = `-direction`, `tMax = +inf`.
   *  - `Point` / `Spot`: ray direction toward the light position,
   *    `tMax = distance(point, lightPosition)`.
   *  - `Ambient`: always `false` (ambient light has no occluder).
   *
   * The caller (integrator) is responsible for offsetting @p point
   * by `surfaceNormal * epsilon` to avoid self-intersection. This
   * interface does **not** take the surface normal: the offset is
   * baked into @p point before invocation.
   *
   * @param [in] point Surface point, already offset by the integrator
   *                   along the surface normal to avoid self-hits.
   * @param [in] scene Scene to query for ray/object intersection.
   * @returns `true` if a ray from @p point toward the light hits any
   *          object before reaching the light.
   */
  [[nodiscard]] virtual bool isOccluded(
      const raytracer::math::Vector3D& point,
      const raytracer::scene::Scene& scene) const = 0;
};
