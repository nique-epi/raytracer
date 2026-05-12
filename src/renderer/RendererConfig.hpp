/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** RendererConfig
*/

#pragma once

#include <memory>
#include "integrator/IIntegrator.hpp"
#include "scene/Scene.hpp"
#include "utils/math/RenderSettings.hpp"

namespace raytracer::core {

/**
 * @brief Long-lived, frame-invariant inputs to the renderer.
 *
 * Built once per render session (or per scene change). Reused across every
 * `render()` call. Pair with `Frame` for the per-frame mutable state.
 */
struct RendererConfig {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
  const scene::Scene& scene;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
  const math::RenderSettings& settings;
  std::shared_ptr<IIntegrator> integrator;
};

}  // namespace raytracer::core
