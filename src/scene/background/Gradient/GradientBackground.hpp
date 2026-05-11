/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** GradientBackground
*/

#pragma once

#include "background/IBackground.hpp"
#include "utils/math/Color.hpp"

namespace raytracer::scene::background {

/**
 * @brief Background that linearly interpolates between two colours along the
 *        vertical axis of the ray direction.
 *
 * The blend factor is derived from the normalised y-component of the ray
 * direction: t = 0.5 * (y + 1). At t=0 the bottom colour is returned; at
 * t=1 the top colour is returned.
 */
class GradientBackground : public IBackground {
 public:
  /**
   * @brief Construct a gradient background.
   *
   * @param [in] bottomColor Colour at the bottom of the gradient (y = -1).
   * @param [in] topColor    Colour at the top of the gradient (y = +1).
   */
  GradientBackground(const raytracer::math::Color& bottomColor,
                     const raytracer::math::Color& topColor);
  ~GradientBackground() override = default;

  GradientBackground(const GradientBackground&) = delete;
  GradientBackground& operator=(const GradientBackground&) = delete;
  GradientBackground(GradientBackground&&) = delete;
  GradientBackground& operator=(GradientBackground&&) = delete;

  /**
   * @brief Return the gradient colour for the given @p ray direction.
   *
   * @param [in] ray Ray whose direction is used to compute the blend factor.
   * @returns Interpolated colour between bottom and top.
   */
  [[nodiscard]] raytracer::math::Color getColor(
      const raytracer::math::Ray& ray) const override;

 private:
  raytracer::math::Color _bottomColor;
  raytracer::math::Color _topColor;
};

}  // namespace raytracer::scene::background
