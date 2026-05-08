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
#include "core/renderer/IRenderer.hpp"
#include "scene/Scene.hpp"
#include "utils/math/Ray.hpp"

namespace raytracer::core {

/**
 * @brief Single-threaded implementation of a renderer.
 */
class MonoThreadRenderer : public IRenderer {
 public:
  MonoThreadRenderer() = default;
  ~MonoThreadRenderer() override = default;

  MonoThreadRenderer(const MonoThreadRenderer&) = delete;
  MonoThreadRenderer(MonoThreadRenderer&&) = delete;
  MonoThreadRenderer& operator=(const MonoThreadRenderer&) = delete;
  MonoThreadRenderer& operator=(MonoThreadRenderer&&) = delete;

  components::Image render(const raytracer::scene::Scene& scene,
                           const ICamera& camera,
                           const math::RenderSettings& settings) override;

  void setProgressCallback(std::function<void(double)> fn) override;

 private:
  static raytracer::math::Color castRay(const raytracer::math::Ray& ray,
                                        const raytracer::scene::Scene& scene,
                                        int depth);
  static raytracer::math::Color computeLighting(
      const raytracer::math::Ray& inRay, const raytracer::math::HitRecord& rec);

  std::function<void(double)> _progressCallback;
};

}  // namespace raytracer::core
