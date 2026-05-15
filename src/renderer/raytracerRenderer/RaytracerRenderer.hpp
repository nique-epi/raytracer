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
 * Shading is **not** implemented here: at the start of every
 * `render()` call the renderer snapshots the active strategy from
 * `config.shadingContext->currentStrategy()` and uses it for the whole
 * frame. A `setStrategy()` performed mid-render takes effect at the
 * next frame.
 *
 * Determinism contract: when `settings.samplesPerPixel == 1` no jitter
 * is applied, so the output is bit-identical regardless of
 * `numThreads`. With `samplesPerPixel > 1` the per-pixel jitter draws
 * from `thread_local` RNGs, so output may vary across thread counts.
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

namespace raytracer::shading {
class IShadingMode;
}  // namespace raytracer::shading

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
   * @p shader is the shading entry point used for every primary ray.
   */
  static void renderTile(const Tile& tile, const scene::Scene& scene,
                         const ICamera& camera,
                         const math::RenderSettings& settings,
                         shading::IShadingMode& shader,
                         components::Image& image);

 private:
  std::function<void(double)> progressCallback_;
  raytracer::common::Logger logger_{"Renderer"};
};

}  // namespace raytracer::core
