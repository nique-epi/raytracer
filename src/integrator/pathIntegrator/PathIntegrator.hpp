/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PathIntegrator
*/

#pragma once

#include "integrator/IIntegrator.hpp"

namespace raytracer::core {

class PathIntegrator : public IIntegrator {
 public:
  PathIntegrator() = default;
  ~PathIntegrator() override = default;

  PathIntegrator(const PathIntegrator&) = delete;
  PathIntegrator& operator=(const PathIntegrator&) = delete;
  PathIntegrator(PathIntegrator&&) = delete;
  PathIntegrator& operator=(PathIntegrator&&) = delete;

  math::Color computeRadiance(const math::Ray& ray, const scene::Scene& scene,
                              int depth) override;
};

}  // namespace raytracer::core
