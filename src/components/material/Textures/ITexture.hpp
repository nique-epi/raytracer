/*
 ** EPITECH PROJECT, 2026
 ** raytracer
 ** File description:
 ** ITexture
 */

/**
 * @file ITexture.hpp
 * @brief Pure interface for texture sampling.
 */

#pragma once

#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

/**
 * @brief Pure interface for texture sampling.
 *
 * A texture provides color values at arbitrary (u, v) coordinates on the
 * texture domain, optionally modulated by the 3-D position @p p for
 * displacement mapping or other effects.
 *
 * @note Implementations should ensure that @p u and @p v are in the range
 *       [0, 1], corresponding to the texture's normalized coordinate space.
 */
class ITexture {
 public:
  /**
   * @brief Virtual destructor.
   */
  virtual ~ITexture();

  ITexture(const ITexture&) = default;
  ITexture& operator=(const ITexture&) = default;
  ITexture(ITexture&&) = default;
  ITexture& operator=(ITexture&&) = default;

  /**
   * @brief Sample the texture at the given coordinates.
   *
   * @param [in] u Texture horizontal coordinate (normalized, [0, 1]).
   * @param [in] v Texture vertical coordinate (normalized, [0, 1]).
   * @param [in] point 3-D position used for texture modulation (e.g., displacement).
   * @returns The sampled color at (u, v) at position @p point.
   */
  [[nodiscard]] virtual raytracer::math::Color sample(
      double u, double v, const raytracer::math::Vector3D& point) const = 0;
};
