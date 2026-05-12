/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MonoThreadRenderer
*/

#include "MonoThreadRenderer.hpp"
#include <limits>
#include <random>
#include <utility>
#include "components/camera/ICamera.hpp"
#include "components/material/IMaterial.hpp"
#include "scene/Scene.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"

namespace raytracer::core {

components::Image MonoThreadRenderer::render(const RendererConfig& config,
                                             const Frame& frame) {
  const auto& scene = config.scene;
  const auto& camera = frame.camera;
  const math::RenderSettings& settings = config.settings;
  const int width = settings.imageWidth;
  const int height = settings.imageHeight;
  components::Image image(width, height);
  constexpr int progressLineInterval = 10;
  // NOLINTNEXTLINE(altera-id-dependent-backward-branch)
  for (int y = 0; y < height; ++y) {
    if (_progressCallback) {
      if (y % progressLineInterval == 0) {
        _progressCallback(static_cast<double>(y) / height);
      }
    }
    // NOLINTNEXTLINE(altera-id-dependent-backward-branch)
    for (int x = 0; x < width; ++x) {
      math::Color accumulated(0, 0, 0);
      // NOLINTNEXTLINE(altera-id-dependent-backward-branch)
      for (int s = 0; s < settings.samplesPerPixel; ++s) {
        thread_local std::mt19937 gen(std::random_device{}());
        thread_local std::uniform_real_distribution<double> dist(0.0, 1.0);
        const double jitterU = (settings.samplesPerPixel > 1) ? dist(gen) : 0.0;
        const double jitterV = (settings.samplesPerPixel > 1) ? dist(gen) : 0.0;
        const double u = (width > 1)
                             ? (static_cast<double>(x) + jitterU) / (width - 1)
                             : 0.5;
        const double v =
            (height > 1)
                ? (static_cast<double>(height - 1 - y) + jitterV) / (height - 1)
                : 0.5;
        const math::Ray ray = camera.getRay(u, v);
        accumulated = accumulated + castRay(ray, scene, settings.maxDepth);
      }
      image.setPixel(
          x, y, accumulated / static_cast<double>(settings.samplesPerPixel));
    }
  }
  if (_progressCallback) {
    _progressCallback(1.0);
  }
  return image;
}

void MonoThreadRenderer::setProgressCallback(std::function<void(double)> fn) {
  _progressCallback = std::move(fn);
}

// NOLINTNEXTLINE(misc-no-recursion)
math::Color MonoThreadRenderer::castRay(const math::Ray& ray,
                                        const scene::Scene& scene, int depth) {
  if (depth <= 0) {
    return {0, 0, 0};
  }
  // NOLINTNEXTLINE(misc-const-correctness)
  math::HitRecord rec;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  if (scene.hit(ray, 0.001, std::numeric_limits<double>::infinity(), rec)) {
    return computeLighting(ray, rec, scene, depth);
  }
  const auto background = scene.getBackground();
  if (background) {
    return background->getColor(ray);
  }
  return {0, 0, 0};
}

// NOLINTNEXTLINE(misc-no-recursion)
math::Color MonoThreadRenderer::computeLighting(const math::Ray& inRay,
                                                const math::HitRecord& rec,
                                                const scene::Scene& scene,
                                                int depth) {
  if (rec.material) {
    math::Color attenuation(0, 0, 0);
    math::Ray scattered(math::Vector3D(0, 0, 0), math::Vector3D(0, 0, 1));
    if (rec.material->scatter(inRay, rec, attenuation, scattered)) {
      return attenuation * castRay(scattered, scene, depth - 1);
    }
    return rec.material->emitted();
  }
  constexpr double half = 0.5;
  return {(rec.normal.x * half) + half, (rec.normal.y * half) + half,
          (rec.normal.z * half) + half};
}

}  // namespace raytracer::core
