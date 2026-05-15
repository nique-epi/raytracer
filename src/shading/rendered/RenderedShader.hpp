/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** RenderedShader
*/

/**
 * @file RenderedShader.hpp
 * @brief Production-grade viewport shader that delegates to an
 *        `IIntegrator`.
 *
 * `RenderedShader` is the bridge between the orthogonal `IShadingMode`
 * and `IIntegrator` interfaces: it satisfies `IShadingMode` by
 * forwarding every primary ray to the wrapped integrator's
 * `computeRadiance`. The integrator owns the rendering equation; this
 * class owns no shading logic of its own.
 *
 * Today the wrapped integrator is always a `WhittedIntegrator`. The
 * day a second integrator lands (path tracer, BDPT, …) only the
 * concrete `IIntegrator` instance changes — `RenderedShader` and every
 * caller above it are unaffected.
 */

#pragma once

#include <memory>

#include "shading/IShadingMode.hpp"

namespace raytracer::core {
class IIntegrator;
}  // namespace raytracer::core

namespace raytracer::shading {

class RenderedShader : public IShadingMode {
 public:
  explicit RenderedShader(std::shared_ptr<core::IIntegrator> integrator);
  ~RenderedShader() override = default;

  RenderedShader(const RenderedShader&) = delete;
  RenderedShader& operator=(const RenderedShader&) = delete;
  RenderedShader(RenderedShader&&) = delete;
  RenderedShader& operator=(RenderedShader&&) = delete;

  math::Color shade(const math::Ray& ray, const scene::Scene& scene,
                    int depth) override;

 private:
  std::shared_ptr<core::IIntegrator> integrator_;
};

}  // namespace raytracer::shading
