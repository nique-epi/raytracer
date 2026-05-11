/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** NoiseTexture
*/

#pragma once

#include "components/material/Textures/ITexture.hpp"

namespace raytracer::materials::textures {

class NoiseTexture : public ITexture {
 public:
  NoiseTexture();
  ~NoiseTexture() override;

  [[nodiscard]] math::Color sample(double u, double v,
                                   const math::Vector3D& point) const override;

 private:
};  // namespace raytracer::class
}  // namespace raytracer::materials::textures
