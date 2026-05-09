/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** NullLightFixture
*/

#pragma once

#include "components/light/ILight.hpp"

class NullLightFixture : public ILight {
 public:
  NullLightFixture() = default;
  ~NullLightFixture() override = default;

  [[nodiscard]] raytracer::math::Color illuminate(
      const raytracer::math::Vector3D& point,
      const raytracer::scene::Scene& scene) const override;

  [[nodiscard]] raytracer::math::Vector3D getDirection(
      const raytracer::math::Vector3D& point) const override;

  [[nodiscard]] double getIntensity() const override;

  [[nodiscard]] bool isOccluded(
      const raytracer::math::Vector3D& point,
      const raytracer::scene::Scene& scene) const override;
};
