/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** CheckerTexture
*/

#pragma once

#include "components/material/Textures/ITexture.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::materials::textures {

/**
 * @brief 3-D procedural checkerboard texture.
 *
 * Alternates between two colors based on the sign of
 * sin(scale * p.x) * sin(scale * p.y) * sin(scale * p.z).
 */
class CheckerTexture : public raytracer::materials::ITexture {
 public:
  /**
   * @brief Construct a checkerboard texture.
   * @param [in] odd   Color used when the sine product is negative.
   * @param [in] even  Color used when the sine product is non-negative.
   * @param [in] scale Frequency of the checker pattern (higher = smaller
   * squares).
   */
  CheckerTexture(const raytracer::math::Color& odd,
                 const raytracer::math::Color& even, double scale);

  CheckerTexture(const CheckerTexture&) = delete;
  CheckerTexture& operator=(const CheckerTexture&) = delete;
  CheckerTexture(CheckerTexture&&) = delete;
  CheckerTexture& operator=(CheckerTexture&&) = delete;
  ~CheckerTexture() override = default;

  /**
   * @brief Sample the checkerboard at the 3-D hit position.
   * @param [in] u     Texture coordinate (unused — pattern is fully 3-D).
   * @param [in] v     Texture coordinate (unused — pattern is fully 3-D).
   * @param [in] point 3-D hit position used to evaluate the checker formula.
   * @returns odd_ or even_ depending on the sine product sign.
   */
  [[nodiscard]] raytracer::math::Color sample(
      double u, double v,
      const raytracer::math::Vector3D& point) const override;

 private:
  raytracer::math::Color odd_;
  raytracer::math::Color even_;
  double scale_;
};

}  // namespace raytracer::materials::textures
