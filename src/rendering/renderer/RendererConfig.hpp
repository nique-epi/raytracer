/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** RendererConfig
*/

#pragma once

#include <memory>
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
};

}  // namespace raytracer::core
