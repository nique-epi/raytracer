/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MonoThreadRenderer
*/

#include "MonoThreadRenderer.hpp"
#include <algorithm>
#include <limits>
#include <random>
#include <utility>
#include "components/camera/ICamera.hpp"
#include "components/light/ILight.hpp"
#include "components/material/IMaterial.hpp"
#include "scene/Scene.hpp"
#include "scene/World.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Constants.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"

namespace raytracer::core {

components::Image MonoThreadRenderer::render(const RendererConfig& config,
                                             const Frame& frame) {
  const scene::Scene& scene = *config.scene;
  const ICamera& camera = *frame.camera;
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
        accumulated =
            accumulated + castRay(ray, scene, settings.maxDepth, true);
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
                                        const scene::Scene& scene, int depth,
                                        bool isPrimary) {
  if (depth <= 0) {
    return {0, 0, 0};
  }
  // NOLINTNEXTLINE(misc-const-correctness)
  math::HitRecord rec;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  if (scene.hit(ray, 0.001, std::numeric_limits<double>::infinity(), rec)) {
    return computeLighting(ray, rec, scene, depth);
  }
  const bool allowEnvironment =
      isPrimary ||
      scene.getWorld().viewportMode() == scene::ViewportMode::MaterialPreview;
  if (allowEnvironment) {
    const auto background = scene.getBackground();
    if (background) {
      return background->getColor(ray);
    }
  }
  return {0, 0, 0};
}

// NOLINTNEXTLINE(misc-no-recursion)
math::Color MonoThreadRenderer::computeLighting(const math::Ray& inRay,
                                                const math::HitRecord& rec,
                                                const scene::Scene& scene,
                                                int depth) {
  if (scene.getWorld().viewportMode() == scene::ViewportMode::Wireframe) {
    constexpr double half = 0.5;
    return {(rec.normal.x * half) + half, (rec.normal.y * half) + half,
            (rec.normal.z * half) + half};
  }
  if (!rec.material) {
    constexpr double half = 0.5;
    return {(rec.normal.x * half) + half, (rec.normal.y * half) + half,
            (rec.normal.z * half) + half};
  }

  if (scene.getWorld().viewportMode() == scene::ViewportMode::MaterialPreview) {
    math::Color attenuation(0, 0, 0);
    math::Ray scattered(math::Vector3D(0, 0, 0), math::Vector3D(0, 0, 1));
    if (rec.material->scatter(inRay, rec, attenuation, scattered)) {
      const math::Color indirect = castRay(scattered, scene, depth - 1, false);
      return rec.material->emitted() + (attenuation * indirect);
    }
    return rec.material->emitted();
  }

  const math::Vector3D shadowOrigin =
      rec.point + (rec.normal * math::constants::shadowRayEpsilon);
  math::Color directLighting(0, 0, 0);
  for (const auto& light : scene.getLights()) {
    const math::Color radiance = light->illuminate(shadowOrigin, scene);
    if (radiance.r == 0 && radiance.g == 0 && radiance.b == 0) {
      continue;
    }
    const math::Vector3D lightDir = light->getDirection(shadowOrigin);
    double cosTheta = 1.0;
    if (lightDir.lengthSquared() > 0.0) {
      cosTheta = std::max(0.0, rec.normal.dot(-lightDir));
      if (cosTheta == 0.0) {
        continue;
      }
    }
    directLighting = directLighting + (radiance * cosTheta);
  }

  math::Color attenuation(0, 0, 0);
  math::Ray scattered(math::Vector3D(0, 0, 0), math::Vector3D(0, 0, 1));
  if (rec.material->scatter(inRay, rec, attenuation, scattered)) {
    const math::Color indirect = castRay(scattered, scene, depth - 1, false);
    return rec.material->emitted() +
           (attenuation * (directLighting + indirect));
  }
  return rec.material->emitted();
}

}  // namespace raytracer::core
