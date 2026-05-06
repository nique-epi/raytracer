/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MonoThreadRenderer
*/

#include "MonoThreadRenderer.hpp"
#include <limits>
#include <utility>
#include "components/material/IMaterial.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Ray.hpp"

namespace raytracer::core {

components::Image MonoThreadRenderer::render(
    const Scene& scene, const ICamera& camera,
    const math::RenderSettings& settings) {
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
      const double u = (width > 1) ? static_cast<double>(x) / (width - 1) : 0.5;
      const double v = (height > 1)
                           ? static_cast<double>(height - 1 - y) / (height - 1)
                           : 0.5;
      const math::Ray ray = camera.getRay(u, v);
      const math::Color pixelColor = castRay(ray, scene, settings.maxDepth);
      image.setPixel(x, y, pixelColor);
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

math::Color MonoThreadRenderer::castRay(const math::Ray& ray,
                                        const Scene& scene, int depth) {
  if (depth <= 0) {
    return {0, 0, 0};
  }
  // NOLINTNEXTLINE(misc-const-correctness)
  math::HitRecord rec;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  if (scene.hits(ray, 0.001, std::numeric_limits<double>::infinity(), rec)) {
    return computeLighting(ray, rec);
  }
  return {0, 0, 0};
}

math::Color MonoThreadRenderer::computeLighting(const math::Ray& inRay,
                                                const math::HitRecord& rec) {
  if (rec.material) {
    math::Color attenuation(0, 0, 0);
    math::Ray scattered(math::Vector3D(0, 0, 0), math::Vector3D(0, 0, 1));
    if (rec.material->scatter(inRay, rec, attenuation, scattered)) {
      return attenuation;
    }
    return rec.material->emitted();
  }
  constexpr double half = 0.5;
  return {(rec.normal.x * half) + half, (rec.normal.y * half) + half,
          (rec.normal.z * half) + half};
}

}  // namespace raytracer::core
