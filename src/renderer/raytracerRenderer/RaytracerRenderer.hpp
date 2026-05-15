/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** RaytracerRenderer
*/

/**
 * @file RaytracerRenderer.hpp
 * @brief Tile-based multi-threaded renderer.
 *
 * Splits the framebuffer into independent tiles via `Tile::subdivide`,
 * then dispatches each tile to a fresh `ThreadPool`. Each worker writes
 * to a disjoint pixel region of the shared `Image` — writes do not need
 * external synchronisation because the regions never overlap.
 *
 * Shading is **not** implemented here: each primary ray is handed off
 * to `config.integrator->computeRadiance(...)`. The renderer's only job
 * is camera sampling, tile dispatch, and writing the resulting pixels.
 * Both this renderer and `MonoThreadRenderer` share the same integrator
 * path so the multithread output is bit-identical to the mono output
 * when `samplesPerPixel == 1`.
 *
 * Determinism contract: when `settings.samplesPerPixel == 1` no jitter
 * is applied, so the output is bit-identical to the mono-thread
 * renderer regardless of `numThreads`. With `samplesPerPixel > 1` the
 * per-pixel jitter draws from `thread_local` RNGs, so output may vary
 * across thread counts; this matches the mono-thread behaviour and is
 * acceptable per the ticket.
 */

#pragma once

#include <cstddef>
#include <functional>
#include "common/helper/Logger.hpp"
#include "components/image/Image.hpp"
#include "renderer/Frame.hpp"
#include "renderer/IRenderer.hpp"
#include "renderer/RendererConfig.hpp"
#include "renderer/Tile.hpp"

namespace raytracer::core {
class IIntegrator;
}  // namespace raytracer::core

namespace raytracer::scene {
class Scene;
}  // namespace raytracer::scene

namespace raytracer::components {
class ICamera;
}  // namespace raytracer::components

namespace raytracer::core {

class RaytracerRenderer : public IRenderer {
 public:
  RaytracerRenderer() = default;
  ~RaytracerRenderer() override = default;

  RaytracerRenderer(const RaytracerRenderer&) = delete;
  RaytracerRenderer(RaytracerRenderer&&) = delete;
  RaytracerRenderer& operator=(const RaytracerRenderer&) = delete;
  RaytracerRenderer& operator=(RaytracerRenderer&&) = delete;

  components::Image render(const RendererConfig& config,
                           const Frame& frame) override;
  void setProgressCallback(std::function<void(double)> fn) override;

  /**
   * @brief Render a single tile in-place into @p image.
   *
   * Public to make per-tile unit testing trivial. Thread-safe with
   * respect to other `renderTile` calls **only if the tiles are
   * disjoint** — the underlying `Image::setPixel` does not synchronise.
   *
   * @p integrator is the shading engine used for every primary ray.
   */
  static void renderTile(const Tile& tile, const scene::Scene& scene,
                         const ICamera& camera,
                         const math::RenderSettings& settings,
                         IIntegrator& integrator, components::Image& image);

 private:
  std::function<void(double)> progressCallback_;
  raytracer::common::Logger logger_{"Renderer"};
};

}  // namespace raytracer::core
