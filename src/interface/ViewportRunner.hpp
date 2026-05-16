/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ViewportRunner
*/

#pragma once

#include <atomic>
#include <mutex>
#include <vector>
#include "common/helper/Logger.hpp"
#include "components/image/Image.hpp"
#include "rendering/renderer/Frame.hpp"
#include "rendering/renderer/RendererConfig.hpp"
#include "rendering/renderer/raytracerRenderer/RaytracerRenderer.hpp"
#include "utils/math/Color.hpp"

namespace raytracer::interface {

class Viewport;

/**
 * @brief Progressive accumulation render loop driven by a live Viewport.
 *
 * Must be driven from the process main thread (macOS Cocoa
 * constraint). `run` opens a `Viewport`, spawns a worker thread that
 * re-renders the scene in successive jittered passes — accumulating
 * into a buffer and pushing the running average to the window — while
 * the main thread pumps the SFML event loop. The (partial or full)
 * averaged image is written to `out.ppm` on exit.
 */
class ViewportRunner {
 public:
  /**
   * @brief Build a runner bound to a renderer and a scene to render.
   *
   * @param [in,out] renderer   Renderer driven once per accumulation
   *                            pass.
   * @param [in] baseConfig     Scene, shading context and target
   *                            settings (copied).
   * @param [in] frame          Per-frame camera state (copied).
   */
  ViewportRunner(raytracer::core::RaytracerRenderer& renderer,
                 raytracer::core::RendererConfig baseConfig,
                 raytracer::core::Frame frame);

  /**
   * @brief Run the accumulation loop to completion.
   *
   * Must be called from the main thread.
   *
   * @returns Process exit status (0 on success).
   */
  int run();

 private:
  static constexpr int samplesPerPass = 2;

  [[nodiscard]] raytracer::core::RendererConfig buildPassConfig() const;
  void accumulate(Viewport& viewport);
  void accumulatePass(std::vector<raytracer::math::Color>& accumulator,
                      const raytracer::components::Image& passImage,
                      raytracer::components::Image& displayImage,
                      int pass) const;
  void publishPass(Viewport& viewport,
                   const raytracer::components::Image& displayImage, int pass);
  void reportPassProgress(Viewport& viewport, int pass, double fraction) const;
  void reportOutcome(Viewport& viewport);

  raytracer::core::RaytracerRenderer& renderer_;
  raytracer::core::RendererConfig baseConfig_;
  raytracer::core::Frame frame_;
  int width_;
  int height_;
  int targetSamples_;
  int totalPasses_;
  int effectiveSamples_;
  std::mutex finalImageMutex_;
  raytracer::components::Image finalImage_;
  std::atomic<int> completedPasses_{0};
  raytracer::common::Logger logger_{"ViewportLoop"};
};

}  // namespace raytracer::interface
