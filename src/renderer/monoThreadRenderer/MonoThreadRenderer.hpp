/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MonoThreadRenderer
*/

/**
 * @file MonoThreadRenderer.hpp
 * @brief Basic single-threaded renderer implementation.
 */

#pragma once

#include <functional>
#include "../IRenderer.hpp"
#include "utils/math/Ray.hpp"

namespace raytracer::math {
class HitRecord;
class Vector3D;
}  // namespace raytracer::math

namespace raytracer::scene {
class Scene;
}  // namespace raytracer::scene

namespace raytracer::core {

/**
 * @brief Single-threaded reference renderer.
 *
 * Dispatches shading per pixel to one of three viewport modes carried by
 * the scene's `World` (Wireframe / MaterialPreview / Rendered). Each mode
 * is implemented as a small helper to keep the recursion logic narrow
 * and the per-mode behaviour obvious.
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
  static raytracer::math::Color castRay(const raytracer::math::Ray& ray,
                                        const scene::Scene& scene, int depth,
                                        bool isPrimary);
  static raytracer::math::Color shade(const raytracer::math::Ray& inRay,
                                      const raytracer::math::HitRecord& rec,
                                      const scene::Scene& scene, int depth);
  static raytracer::math::Color shadeWireframe(
      const raytracer::math::HitRecord& rec);
  static raytracer::math::Color shadeMaterialPreview(
      const raytracer::math::Ray& inRay, const raytracer::math::HitRecord& rec,
      const scene::Scene& scene, int depth);
  static raytracer::math::Color shadeRendered(
      const raytracer::math::Ray& inRay, const raytracer::math::HitRecord& rec,
      const scene::Scene& scene, int depth);

  std::function<void(double)> progressCallback_;
};

}  // namespace raytracer::core
