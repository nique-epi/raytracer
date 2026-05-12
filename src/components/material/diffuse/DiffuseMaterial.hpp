/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** DiffuseMaterial
*/

#pragma once

#include "components/material/abstract/AMaterial.hpp"
#include "utils/math/Color.hpp"

namespace raytracer::components::material {

/**
 * @brief Lambertian diffuse material.
 *
 * Scatters incoming rays uniformly in the hemisphere above the surface
 * normal. The attenuation is always equal to the albedo.
 * Inherits the default non-emissive emitted() from AMaterial.
 */
class DiffuseMaterial : public AMaterial {
 public:
  explicit DiffuseMaterial(const raytracer::math::Color& albedo);

  DiffuseMaterial(const DiffuseMaterial&) = delete;
  DiffuseMaterial& operator=(const DiffuseMaterial&) = delete;
  DiffuseMaterial(DiffuseMaterial&&) = delete;
  DiffuseMaterial& operator=(DiffuseMaterial&&) = delete;
  ~DiffuseMaterial() override = default;

  bool scatter(const raytracer::math::Ray& in,
               const raytracer::math::HitRecord& rec,
               raytracer::math::Color& attenuation,
               raytracer::math::Ray& scattered) const override;

  [[nodiscard]] raytracer::math::Color diffuseAlbedo() const override;

 private:
  raytracer::math::Color albedo;
};

}  // namespace raytracer::components::material
