/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Glass
*/

#pragma once

#include "components/material/abstract/AMaterial.hpp"

namespace raytracer::components::material {

class Glass : public AMaterial {
 public:
  Glass();
  explicit Glass(double refractionIndex);
  ~Glass() override = default;

  Glass(const Glass&) = delete;
  Glass& operator=(const Glass&) = delete;
  Glass(Glass&&) = delete;
  Glass& operator=(Glass&&) = delete;

  bool scatter(const raytracer::math::Ray& in,
               const raytracer::math::HitRecord& rec,
               raytracer::math::Color& attenuation,
               raytracer::math::Ray& scattered) const override;

  void setRefractionIndex(double refractionIndex);

 private:
  double refractionIndex_;
};
}  // namespace raytracer::components::material
