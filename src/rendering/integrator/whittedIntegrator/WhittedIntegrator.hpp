/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** WhittedIntegrator
*/

/**
 * @file WhittedIntegrator.hpp
 * @brief Whitted-style ray-tracing integrator.
 *
 * Solves the rendering equation for a primary ray using the classic
 * Whitted-1980 recipe: direct lighting from every `ILight` (Lambert
 * cosine + shadow ray + inverse-square falloff), plus an indirect
 * bounce obtained by sampling the material's scatter ray and recursing
 * up to `depth` levels.
 *
 * Scope: this class only implements the "Rendered" viewport-mode
 * pipeline. Viewport-mode dispatching (Wireframe / MaterialPreview /
 * Rendered) lives in the `shading::` layer; the integrator is wrapped
 * by `RenderedShader` and never sees the other modes.
 *
 * Background policy: the integrator samples the scene background for
 * the primary ray only. Secondary scatter rays that escape the scene
 * return black (no environment lighting), matching the Rendered mode
 * contract documented on `ViewportMode::Rendered`.
 */

#pragma once

#include "rendering/integrator/IIntegrator.hpp"

namespace raytracer::core {

class WhittedIntegrator : public IIntegrator {
 public:
  WhittedIntegrator() = default;
  ~WhittedIntegrator() override = default;

  WhittedIntegrator(const WhittedIntegrator&) = delete;
  WhittedIntegrator& operator=(const WhittedIntegrator&) = delete;
  WhittedIntegrator(WhittedIntegrator&&) = delete;
  WhittedIntegrator& operator=(WhittedIntegrator&&) = delete;

  math::Color computeRadiance(const math::Ray& ray, const scene::Scene& scene,
                              int depth) override;
};

}  // namespace raytracer::core
