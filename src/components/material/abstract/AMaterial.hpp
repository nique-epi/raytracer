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
  /**
   * @brief Construct an AMaterial with optional Phong specular parameters.
   *
   * Defaults (@c Color(0, 0, 0) and @c 1.0) collapse the specular term in
   * @c brdf to zero, so subclasses that don't pass any value behave as
   * pure Lambert just like before this contract was widened.
   *
   * @param [in] specularAlbedo Phong @c ks. @c Color(0, 0, 0) disables the
   *                            highlight.
   * @param [in] shininess      Phong @c alpha. Must be strictly positive.
   */
  explicit AMaterial(raytracer::math::Color specularAlbedo
                         = raytracer::math::Color(0.0, 0.0, 0.0),
                     double shininess = 1.0);
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
   * @brief Return the Phong @c ks stored at construction.
   *
   * @returns Linear RGB specular reflectance constant.
   */
  [[nodiscard]] raytracer::math::Color specularAlbedo() const override;

  /**
   * @brief Return the Phong shininess stored at construction.
   *
   * @returns Phong exponent applied to @c max(0, R·V) in @c brdf.
   */
  [[nodiscard]] double shininess() const override;

  /**
   * @brief Universal Phong BRDF: Lambert diffuse + Phong specular highlight.
   *
   * Computes @c diffuseAlbedo()/PI + @c specularAlbedo() *
   * @c max(0, R·V)^shininess() where @c R is the reflection of the incoming
   * light direction about the surface normal. When @c specularAlbedo()
   * returns black (the default), this collapses to pure Lambert
   * @c diffuseAlbedo()/PI.
   *
   * Subclasses that need a different direct-lighting response (e.g.
   * @c PrincipledMaterial with Blinn-Phong + Schlick Fresnel) override
   * this method.
   *
   * @param [in] incomingDirection Unit direction from the light toward the
   *                               shaded point.
   * @param [in] outgoingDirection Unit direction from the shaded point
   *                               toward the camera.
   * @param [in] normal            Unit surface normal at the shaded point.
   * @returns @c diffuseAlbedo()/PI plus Phong specular contribution.
   */
  [[nodiscard]] raytracer::math::Color brdf(
      const raytracer::math::Vector3D& incomingDirection,
      const raytracer::math::Vector3D& outgoingDirection,
      const raytracer::math::Vector3D& normal) const override;

 private:
  raytracer::math::Color specularAlbedo_;
  double shininess_;
};

#endif  // MATERIAL_ABSTRACT_AMATERIAL_HPP_
