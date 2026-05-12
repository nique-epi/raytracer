/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SolidColor
*/

#pragma once

#include "components/material/Textures/ITexture.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::materials::textures {

/**
 * @brief Texture that returns a single constant color regardless of
 * coordinates.
 */
class SolidColor : public raytracer::materials::ITexture {
 public:
  /**
   * @brief Construct a solid-color texture.
   * @param [in] color The constant color returned by sample().
   */
  explicit SolidColor(const raytracer::math::Color& color);

  SolidColor(const SolidColor&) = delete;
  SolidColor& operator=(const SolidColor&) = delete;
  SolidColor(SolidColor&&) = delete;
  SolidColor& operator=(SolidColor&&) = delete;
  ~SolidColor() override = default;

  /**
   * @brief Sample the texture — always returns the stored color.
   * @param [in] u Horizontal texture coordinate (unused).
   * @param [in] v Vertical texture coordinate (unused).
   * @param [in] point 3-D hit position (unused).
   * @returns The constant color.
   */
  [[nodiscard]] raytracer::math::Color sample(
      double u, double v,
      const raytracer::math::Vector3D& point) const override;

 private:
  raytracer::math::Color color_;
};

}  // namespace raytracer::materials::textures
