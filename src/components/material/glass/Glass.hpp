/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Glass
*/

#pragma once

#include "components/material/abstract/AMaterial.hpp"
#include "utils/math/Color.hpp"

namespace raytracer::components::material {

class Glass : public AMaterial {
 public:
  Glass();
  Glass(double refractionIndex, const math::Color& albedo, double transparency)
      : refractionIndex_(refractionIndex),
        albedo_(albedo),
        transparency_(transparency) {}
  ~Glass() override = default;

  Glass(const Glass&) = delete;
  Glass& operator=(const Glass&) = delete;
  Glass(Glass&&) = delete;
  Glass& operator=(Glass&&) = delete;

  bool scatter(const raytracer::math::Ray& in,
               const raytracer::math::HitRecord& rec,
               raytracer::math::Color& attenuation,
               raytracer::math::Ray& scattered) const override;

 private:
  double refractionIndex_;
  raytracer::math::Color albedo_;
  double transparency_;
};
}  // namespace raytracer::components::material
