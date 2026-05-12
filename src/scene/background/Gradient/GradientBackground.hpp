/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** GradientBackground
*/

#pragma once

#include "scene/background/IBackground.hpp"

namespace raytracer::scene::background {

/**
 * @brief Background implementation that blends two colours vertically.
 *
 * Rays with a downward direction sample the top colour, while upward rays
 * sample the bottom colour. Intermediate directions return a linear blend.
 */
class GradientBackground : public IBackground {
 public:
  GradientBackground() = default;
  /**
   * @brief Construct a gradient background from two endpoint colours.
   *
   * @param [in] topColor Colour returned for downward rays.
   * @param [in] bottomColor Colour returned for upward rays.
   */
  GradientBackground(const raytracer::math::Color& topColor,
                     const raytracer::math::Color& bottomColor)
      : topColor_(topColor), bottomColor_(bottomColor) {};
  ~GradientBackground() override = default;

  GradientBackground(const GradientBackground&) = delete;
  GradientBackground& operator=(const GradientBackground&) = delete;
  GradientBackground(GradientBackground&&) = delete;
  GradientBackground& operator=(GradientBackground&&) = delete;

  /**
   * @brief Return the interpolated gradient colour for @p ray.
   *
   * @inheritDoc
   */
  [[nodiscard]] raytracer::math::Color getColor(
      const raytracer::math::Ray& ray) const override;

 private:
  raytracer::math::Color topColor_ = raytracer::math::Color(1.0, 1.0, 1.0);
  raytracer::math::Color bottomColor_ = raytracer::math::Color(0, 0, 0);
};
}  // namespace raytracer::scene::background
