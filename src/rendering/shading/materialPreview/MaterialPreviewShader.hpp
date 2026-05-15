/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MaterialPreviewShader
*/

/**
 * @file MaterialPreviewShader.hpp
 * @brief Env-light-only material preview shader.
 *
 * Implements Blender's "Material Preview" semantics: every scene
 * `ILight` is ignored, and the scene background acts as an infinite
 * environment light sampled on **both** primary and secondary rays.
 * Materials still scatter and recurse, so the shader respects the
 * material's BRDF and emission terms.
 *
 * Recursion is bounded by `depth`: scattered rays are followed until
 * `depth` reaches zero or the ray escapes into the background.
 */

#pragma once

#include "rendering/shading/IShadingMode.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Ray.hpp"

namespace raytracer::scene {
class Scene;
}  // namespace raytracer::scene

namespace raytracer::shading {

class MaterialPreviewShader : public IShadingMode {
 public:
  MaterialPreviewShader() = default;
  ~MaterialPreviewShader() override = default;

  MaterialPreviewShader(const MaterialPreviewShader&) = delete;
  MaterialPreviewShader& operator=(const MaterialPreviewShader&) = delete;
  MaterialPreviewShader(MaterialPreviewShader&&) = delete;
  MaterialPreviewShader& operator=(MaterialPreviewShader&&) = delete;

  math::Color shade(const math::Ray& ray, const scene::Scene& scene,
                    int depth) override;

 private:
  /**
   * @brief Non-virtual recursive helper that owns the actual shading
   *        loop (intersect → scatter → recurse).
   *
   * `static` so it cannot accidentally be overridden by a subclass and
   * so the recursive call site stays a direct call — inlinable by the
   * compiler, no per-bounce vtable lookup. `shade()` dispatches here
   * once per primary ray; everything below stays non-virtual.
   *
   * @param[in] ray   Ray currently being traced (primary or scattered).
   * @param[in] scene Scene queried for hits and background.
   * @param[in] depth Remaining bounce budget.
   * @returns Linear-space radiance along @p ray.
   */
  static math::Color traceRay(const math::Ray& ray,
                              const scene::Scene& scene, int depth);
};

}  // namespace raytracer::shading
