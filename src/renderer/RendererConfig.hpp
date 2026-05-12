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

namespace raytracer::core {

class IIntegrator;

/**
 * @brief Long-lived, frame-invariant inputs to the renderer.
 *
 * Built once per render session (or per scene change) and reused across
 * every `render()` call. Pair with `Frame` for the per-frame state.
 *
 * Owns its inputs by value or `shared_ptr` — no reference members — so a
 * config built from locals cannot dangle when reused across a render loop.
 */
struct RendererConfig {
  std::shared_ptr<const scene::Scene> scene;
  math::RenderSettings settings;
  std::shared_ptr<IIntegrator> integrator;
};

}  // namespace raytracer::core
