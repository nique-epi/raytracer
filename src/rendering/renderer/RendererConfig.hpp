/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** RendererConfig
*/

#pragma once

#include <memory>
#include <string>
#include "utils/math/RenderSettings.hpp"

namespace raytracer::scene {
class Scene;
}  // namespace raytracer::scene

namespace raytracer::shading {
class ShadingContext;
}  // namespace raytracer::shading

namespace raytracer::core {

/**
 * @brief Long-lived, frame-invariant inputs to the renderer.
 *
 * Built once per render session (or per scene change) and reused across
 * every `render()` call. Pair with `Frame` for the per-frame state.
 *
 * Owns its inputs by value or `shared_ptr` — no reference members — so a
 * config built from locals cannot dangle when reused across a render loop.
 *
 * `shadingContext` is the Strategy-pattern Context the renderer talks
 * to. The renderer snapshots its active strategy once per `render()`
 * call, so a `setStrategy()` performed by the UI mid-render takes
 * effect at the next frame, never within the current one.
 */
struct RendererConfig {
  std::shared_ptr<const scene::Scene> scene;
  math::RenderSettings settings;
  std::shared_ptr<shading::ShadingContext> shadingContext;
  /**
   * @brief Output file path used by the renderer or viewport runner when
   *        saving the final image. Defaults to `"out.ppm"`.
   *
   * Set from the JSON render-configuration `outputFile` field
   * (BUILD_BONUS). Propagated to both the headless and viewport paths so
   * the two code-paths write to the same destination.
   */
  std::string outputPath{"out.ppm"};
};

}  // namespace raytracer::core
