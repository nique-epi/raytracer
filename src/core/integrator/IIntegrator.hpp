/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** IIntegrator
*/

#pragma once

#include "components/light/ILight.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Ray.hpp"
namespace raytracer::core {

class IIntegrator {
 public:
  virtual ~IIntegrator() = default;
  virtual math::Color Li(const math::Ray& ray, const scene::Scene& scene,
                         int depth) = 0;
};
}  // namespace raytracer::core
