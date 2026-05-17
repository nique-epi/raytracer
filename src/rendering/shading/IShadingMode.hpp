/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** IShadingMode
*/

/**
 * @file IShadingMode.hpp
 * @brief Interface for a viewport shading mode.
 *
 * An `IShadingMode` turns a single primary ray into a radiance value.
 * It is the renderer's single entry point per pixel and per sample:
 * the renderer never calls an `IIntegrator` directly, it always goes
 * through this interface.
 *
 * Three concrete shaders ship today, one per `ViewportMode`:
 *  - `WireframeShader`        — Blender-style flat grey clay preview.
 *  - `MaterialPreviewShader`  — env-light-only preview.
 *  - `RenderedShader`         — delegates to an `IIntegrator`
 *                               (currently `WhittedIntegrator`).
 *
 * Picking the right shader for the scene's active `ViewportMode` is
 * the job of the application layer: `Application::createShadingContext`
 * inspects `World::viewportMode()` and wraps the selected concrete
 * `IShadingMode` in a `ShadingContext`. The renderer then resolves the
 * active strategy through that context once per `render()`, so the rest
 * of the pipeline only ever talks to this interface.
 *
 * Background and recursion policies are owned by each shader: the
 * interface does not constrain them.
 */

#pragma once

#include "utils/math/Color.hpp"
#include "utils/math/Ray.hpp"

namespace raytracer::scene {
class Scene;
}  // namespace raytracer::scene

namespace raytracer::shading {

class IShadingMode {
 public:
  IShadingMode() = default;
  virtual ~IShadingMode() = default;
  IShadingMode(const IShadingMode&) = delete;
  IShadingMode& operator=(const IShadingMode&) = delete;
  IShadingMode(IShadingMode&&) = delete;
  IShadingMode& operator=(IShadingMode&&) = delete;

  virtual math::Color shade(const math::Ray& ray, const scene::Scene& scene,
                            int depth) = 0;
};

}  // namespace raytracer::shading
