/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** RaytracerRenderer
*/

#include "RaytracerRenderer.hpp"

#include <atomic>
#include <future>
#include <random>
#include <utility>
#include <vector>

#include "components/camera/ICamera.hpp"
#include "os/threads/ThreadPool.hpp"
#include "rendering/renderer/RendererException.hpp"
#include "scene/Scene.hpp"
#include "rendering/shading/IShadingMode.hpp"
#include "rendering/shading/ShadingContext.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Ray.hpp"

namespace raytracer::core {

components::Image RaytracerRenderer::render(const RendererConfig& config,
                                            const Frame& frame) {
  if (!config.shadingContext) {
    throw RendererException(
        "RaytracerRenderer::render requires RendererConfig.shadingContext");
  }
  const std::shared_ptr<shading::IShadingMode> strategy =
      config.shadingContext->currentStrategy();
  if (!strategy) {
    throw RendererException(
        "RaytracerRenderer::render received a ShadingContext with no "
        "active strategy");
  }
  const scene::Scene& scene = *config.scene;
  const ICamera& camera = *frame.camera;
  const math::RenderSettings& settings = config.settings;
  shading::IShadingMode& shader = *strategy;

  auto renderTimer = logger_.scope("render()");
  logger_.info("starting render ", settings.imageWidth, 'x',
               settings.imageHeight, ", tiles ", settings.tileWidth, 'x',
               settings.tileHeight, ", samples=", settings.samplesPerPixel,
               ", maxDepth=", settings.maxDepth);

  components::Image image(settings.imageWidth, settings.imageHeight);
  const std::vector<Tile> tiles =
      Tile::subdivide(settings.imageWidth, settings.imageHeight,
                      settings.tileWidth, settings.tileHeight);
  if (tiles.empty()) {
    logger_.warn(
        "no tiles produced — empty image returned (invalid settings?)");
    if (progressCallback_) {
      progressCallback_(1.0);
    }
    return image;
  }
  os::threads::ThreadPool pool(settings.numThreads);
  logger_.debug("ThreadPool spawned ", pool.workerCount(), " worker(s) for ",
                tiles.size(), " tile(s)");

  std::atomic<std::size_t> completedTiles{0};
  const std::size_t totalTiles = tiles.size();

  std::vector<std::future<void>> futures;
  futures.reserve(totalTiles);
  for (const Tile& tile : tiles) {
    futures.emplace_back(pool.submit([tile, &scene, &camera, &settings,
                                      &shader, &image, &completedTiles,
                                      totalTiles, this] {
      renderTile(tile, scene, camera, settings, shader, image);
      const std::size_t done =
          completedTiles.fetch_add(1, std::memory_order_relaxed) + 1;
      if (progressCallback_) {
        progressCallback_(static_cast<double>(done) /
                          static_cast<double>(totalTiles));
      }
    }));
  }

  for (auto& future : futures) {
    future.get();
  }

  if (progressCallback_) {
    progressCallback_(1.0);
  }
  return image;
}

void RaytracerRenderer::setProgressCallback(std::function<void(double)> fn) {
  progressCallback_ = std::move(fn);
}

void RaytracerRenderer::renderTile(const Tile& tile, const scene::Scene& scene,
                                   const ICamera& camera,
                                   const math::RenderSettings& settings,
                                   shading::IShadingMode& shader,
                                   components::Image& image) {
  const int width = settings.imageWidth;
  const int height = settings.imageHeight;
  const int xEnd = tile.x + tile.width;
  const int yEnd = tile.y + tile.height;

  for (int y = tile.y; y < yEnd; ++y) {
    for (int x = tile.x; x < xEnd; ++x) {
      math::Color accumulated(0, 0, 0);
      for (int sample = 0; sample < settings.samplesPerPixel; ++sample) {
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
            accumulated + shader.shade(ray, scene, settings.maxDepth);
      }
      image.setPixel(
          x, y, accumulated / static_cast<double>(settings.samplesPerPixel));
    }
  }
}

}  // namespace raytracer::core
