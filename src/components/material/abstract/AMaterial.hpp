/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** AMaterial
*/

/**
 * @file AMaterial.hpp
 * @brief Abstract base class for non-emissive materials.
 */

#ifndef MATERIAL_ABSTRACT_AMATERIAL_HPP_
#define MATERIAL_ABSTRACT_AMATERIAL_HPP_

#include "components/material/IMaterial.hpp"

/**
 * @brief Abstract base class that implements IMaterial with a default
 *        non-emissive behaviour.
 *
 * Concrete materials should inherit from AMaterial rather than IMaterial
 * directly. emitted() returns black by default so non-emissive materials
 * (Lambertian, Metal, Glass, …) do not need to override it.
 *
 * Only emissive materials (e.g. DiffuseLight) need to override emitted().
 */
class AMaterial : public IMaterial {
 public:
  AMaterial() = default;
  ~AMaterial() override = default;

  AMaterial(const AMaterial&) = delete;
  AMaterial& operator=(const AMaterial&) = delete;
  AMaterial(AMaterial&&) = delete;
  AMaterial& operator=(AMaterial&&) = delete;

  /**
   * @brief Return black, meaning this material emits no light.
   *
   * Override this method in emissive materials to return the emitted radiance.
   *
   * @returns Color(0, 0, 0).
   */
  [[nodiscard]] raytracer::math::Color emitted() const override;

  /**
   * @brief Default diffuse albedo for non-Lambertian materials.
   *
   * Returns black so specular (mirror, glass) materials do not receive
   * Lambert direct lighting. Lambert-style materials override this to
   * return their albedo.
   *
   * @returns Color(0, 0, 0).
   */
  [[nodiscard]] raytracer::math::Color diffuseAlbedo() const override;

  /**
   * @brief Default Lambertian BRDF: @c diffuseAlbedo() / PI.
   *
   * Materials whose direct-lighting response is purely Lambertian inherit
   * this implementation; they only need to override @c diffuseAlbedo() to
   * return the correct value. Materials that add other reflection terms
   * (e.g. Phong specular highlight) override this method to add the extra
   * contribution on top of the diffuse base.
   *
   * @param [in] incomingDirection Unit direction from the light toward the
   *                               shaded point.
   * @param [in] outgoingDirection Unit direction from the shaded point
   *                               toward the camera.
   * @param [in] normal            Unit surface normal at the shaded point.
   * @returns @c diffuseAlbedo() divided by @c PI.
   */
  [[nodiscard]] raytracer::math::Color brdf(
      const raytracer::math::Vector3D& incomingDirection,
      const raytracer::math::Vector3D& outgoingDirection,
      const raytracer::math::Vector3D& normal) const override;
};

#endif  // MATERIAL_ABSTRACT_AMATERIAL_HPP_
