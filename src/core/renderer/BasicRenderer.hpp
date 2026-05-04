/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** BasicRenderer
*/

/**
 * @file BasicRenderer.hpp
 * @brief Basic single-threaded renderer implementation.
 */

#ifndef CORE_RENDERER_BASICRENDERER_HPP_
#define CORE_RENDERER_BASICRENDERER_HPP_

#include <functional>
#include "components/Primitives/Collection.hpp"
#include "components/camera/ICamera.hpp"
#include "components/image/Image.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/RenderSettings.hpp"

namespace raytracer::core {

/**
 * @brief Single-threaded implementation of a renderer.
 */
class BasicRenderer {
 public:
  BasicRenderer() = default;
  ~BasicRenderer() = default;

  BasicRenderer(const BasicRenderer&) = delete;
  BasicRenderer(BasicRenderer&&) = delete;
  BasicRenderer& operator=(const BasicRenderer&) = delete;
  BasicRenderer& operator=(BasicRenderer&&) = delete;

  components::Image render(const components::Collection& scene,
                           const ICamera& camera,
                           const math::RenderSettings& settings);

  void setProgressCallback(std::function<void(double)> fn);

 private:
  static raytracer::math::Color castRay(const raytracer::math::Ray& ray,
                                        const components::Collection& scene,
                                        int depth);
  static raytracer::math::Color computeLighting(
      const raytracer::math::Ray& inRay, const raytracer::math::HitRecord& rec);

  std::function<void(double)> _progressCallback;
};

}  // namespace raytracer::core

#endif  // CORE_RENDERER_BASICRENDERER_HPP_
