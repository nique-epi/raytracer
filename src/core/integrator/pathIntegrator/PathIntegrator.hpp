/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PathIntegrator
*/

#pragma once

#include "core/integrator/IIntegrator.hpp"
using IIntegrator = raytracer::core::IIntegrator;

class PathIntegrator : public IIntegrator {
 public:
  PathIntegrator() = default;
  ~PathIntegrator() override = default;

  PathIntegrator(const PathIntegrator&) = delete;
  PathIntegrator& operator=(const PathIntegrator&) = delete;
  PathIntegrator(PathIntegrator&&) = delete;
  PathIntegrator& operator=(PathIntegrator&&) = delete;

  raytracer::math::Color Li(const raytracer::math::Ray& ray,
                            const raytracer::scene::Scene& scene,
                            int depth) override;
};
