/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SolidBackground
*/

#pragma once

#include "background/IBackground.hpp"
#include "utils/math/Color.hpp"

namespace raytracer::scene::background {

/**
 * @brief Background implementation that returns a single constant colour.
 *
 * This class represents a solid (uniform) background: every ray that misses
 * the scene returns the same colour regardless of direction.
 */
class SolidBackground : public IBackground {
 public:
  /**
   * @brief Construct a solid background with given @p color.
   *
   * @param [in] color Colour to return for all rays.
   */
  explicit SolidBackground(const raytracer::math::Color& color);
  ~SolidBackground() override = default;

  SolidBackground(const SolidBackground&) = delete;
  SolidBackground& operator=(const SolidBackground&) = delete;
  SolidBackground(SolidBackground&&) = delete;
  SolidBackground& operator=(SolidBackground&&) = delete;

  /**
   * @brief Return the stored solid colour for @p ray.
   *
   * @inheritDoc
   */
  [[nodiscard]] raytracer::math::Color getColor(
      const raytracer::math::Ray& ray) const override;

 private:
  raytracer::math::Color _color;
};

}  // namespace raytracer::scene::background
