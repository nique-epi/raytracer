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

class SolidBackground : public IBackground {
 public:
  explicit SolidBackground(const raytracer::math::Color& color);
  ~SolidBackground() override = default;

  SolidBackground(const SolidBackground&) = delete;
  SolidBackground& operator=(const SolidBackground&) = delete;
  SolidBackground(SolidBackground&&) = delete;
  SolidBackground& operator=(SolidBackground&&) = delete;

  [[nodiscard]] raytracer::math::Color getColor(
      const raytracer::math::Ray& ray) const override;

 private:
  raytracer::math::Color _color;
};

}  // namespace raytracer::scene::background
