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
};

#endif  // MATERIAL_ABSTRACT_AMATERIAL_HPP_
