/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** GradientBackground
*/

#pragma once

#include "scene/background/IBackground.hpp"

namespace raytracer::scene::background {

class GradientBackground : public IBackground {
 public:
  GradientBackground();
  GradientBackground(const raytracer::math::Color& topColor,
                     const raytracer::math::Color& bottomColor)
      : topColor_(topColor), bottomColor_(bottomColor) {};
  ~GradientBackground() override;

  GradientBackground(const GradientBackground&) = delete;
  GradientBackground& operator=(const GradientBackground&) = delete;
  GradientBackground(GradientBackground&&) = delete;
  GradientBackground& operator=(GradientBackground&&) = delete;

  [[nodiscard]] raytracer::math::Color getColor(
      const raytracer::math::Ray& ray) const override;

 private:
  raytracer::math::Color topColor_ = raytracer::math::Color(1.0, 1.0, 1.0);
  raytracer::math::Color bottomColor_ = raytracer::math::Color(0, 0, 0);
};
}  // namespace raytracer::scene::background
