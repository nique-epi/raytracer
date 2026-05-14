/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MonoThreadRenderer
*/

#include "MonoThreadRenderer.hpp"

#include <random>
#include <stdexcept>
#include <utility>

#include "components/camera/ICamera.hpp"
#include "integrator/IIntegrator.hpp"
#include "scene/Scene.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Ray.hpp"

namespace raytracer::core {

components::Image MonoThreadRenderer::render(const RendererConfig& config,
                                             const Frame& frame) {
  if (!config.integrator) {
    throw std::invalid_argument(
        "MonoThreadRenderer::render requires RendererConfig.integrator");
  }
  const scene::Scene& scene = *config.scene;
  const ICamera& camera = *frame.camera;
  const math::RenderSettings& settings = config.settings;
  IIntegrator& integrator = *config.integrator;
  const int width = settings.imageWidth;
  const int height = settings.imageHeight;
  components::Image image(width, height);
  constexpr int progressLineInterval = 10;
  // NOLINTNEXTLINE(altera-id-dependent-backward-branch)
  for (int y = 0; y < height; ++y) {
    if (progressCallback_) {
      if (y % progressLineInterval == 0) {
        progressCallback_(static_cast<double>(y) / height);
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
        accumulated = accumulated +
                      integrator.computeRadiance(ray, scene, settings.maxDepth);
      }
      image.setPixel(
          x, y, accumulated / static_cast<double>(settings.samplesPerPixel));
    }
  }
  if (progressCallback_) {
    progressCallback_(1.0);
  }
  return image;
}

void MonoThreadRenderer::setProgressCallback(std::function<void(double)> fn) {
  progressCallback_ = std::move(fn);
}

}  // namespace raytracer::core
