/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Glossy
*/

#pragma once

#include <algorithm>
#include "components/material/abstract/AMaterial.hpp"
#include "utils/math/Color.hpp"

namespace raytracer::components::material {

class Glossy : public AMaterial {
 public:
  Glossy() = default;
  Glossy(double fuzz, const math::Color& albedo);
  ~Glossy() override = default;

  Glossy(const Glossy&) = delete;
  Glossy& operator=(const Glossy&) = delete;
  Glossy(Glossy&&) = delete;
  Glossy& operator=(Glossy&&) = delete;

  bool scatter(const raytracer::math::Ray& in,
               const raytracer::math::HitRecord& rec,
               raytracer::math::Color& attenuation,
               raytracer::math::Ray& scattered) const override;

  void setFuzz(double fuzz);
  void setAlbedo(const math::Color& albedo);

 private:
  double fuzz{0.0};
  math::Color albedo{1.0, 1.0, 1.0};
};
}  // namespace raytracer::components::material
