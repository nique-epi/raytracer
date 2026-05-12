/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** IBackground
*/

#pragma once

#include "utils/math/Color.hpp"
#include "utils/math/Ray.hpp"

namespace raytracer::scene::background {

/**
 * @brief Abstract interface for scene background providers.
 *
 * Implementations must provide a colour for a given ray that escapes the
 * scene (i.e. samples the infinite background). Implementations should be
 * lightweight and trivially copyable is not required.
 */
class IBackground {
 public:
  IBackground() = default;
  virtual ~IBackground() = default;

  IBackground(const IBackground&) = delete;
  IBackground& operator=(const IBackground&) = delete;
  IBackground(IBackground&&) = delete;
  IBackground& operator=(IBackground&&) = delete;

  /**
   * @brief Return the background colour seen along @p ray.
   *
   * Implementations provide the colour that should be returned when a ray
   * does not intersect any scene geometry.
   *
   * @param [in] ray Ray in world space used to sample the background.
   * @returns The colour corresponding to the given ray.
   */
  [[nodiscard]] virtual raytracer::math::Color getColor(
      const raytracer::math::Ray& ray) const = 0;
};

}  // namespace raytracer::scene::background
