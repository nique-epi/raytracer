/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MonoThreadRenderer
*/

/**
 * @file MonoThreadRenderer.hpp
 * @brief Single-threaded renderer. Shading is delegated to the
 *        `IIntegrator` carried by `RendererConfig`; this class only
 *        owns the per-pixel sampling loop.
 */

#pragma once

#include <functional>
#include "../IRenderer.hpp"

namespace raytracer::core {

/**
 * @brief Single-threaded reference renderer.
 *
 * Walks every pixel in row-major order and asks
 * `config.integrator->computeRadiance(...)` for the radiance carried by
 * the primary ray. All shading, viewport-mode dispatch, and recursive
 * bouncing live in the integrator — the renderer is intentionally a
 * thin driver over the camera + integrator pair.
 *
 * Precondition: `config.integrator != nullptr`. Renderers throw if it
 * is missing rather than silently falling back to a built-in shading
 * path: shading lives in the integrator, never duplicated here.
 */
class MonoThreadRenderer : public IRenderer {
 public:
  MonoThreadRenderer() = default;
  ~MonoThreadRenderer() override = default;

  MonoThreadRenderer(const MonoThreadRenderer&) = delete;
  MonoThreadRenderer(MonoThreadRenderer&&) = delete;
  MonoThreadRenderer& operator=(const MonoThreadRenderer&) = delete;
  MonoThreadRenderer& operator=(MonoThreadRenderer&&) = delete;

  components::Image render(const RendererConfig& config,
                           const Frame& frame) override;

  void setProgressCallback(std::function<void(double)> fn) override;

 private:
  std::function<void(double)> progressCallback_;
};

}  // namespace raytracer::core
