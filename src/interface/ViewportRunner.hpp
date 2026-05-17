/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ViewportRunner
*/

#pragma once

#include <atomic>
#include <mutex>
#include <string>
#include <vector>
#include "common/helper/Logger.hpp"
#include "components/image/Image.hpp"
#include "rendering/renderer/Frame.hpp"
#include "rendering/renderer/RendererConfig.hpp"
#include "rendering/renderer/raytracerRenderer/RaytracerRenderer.hpp"
#include "scene/World.hpp"
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

  /**
   * @brief Apply a pending viewport-mode switch, if any.
   *
   * Consumes the viewport's pending mode request: when present it
   * swaps the shading strategy, discards all accumulation state and
   * repaints the window blank so rendering restarts from scratch in
   * the new mode.
   *
   * @param[in,out] viewport Viewport polled for the pending request.
   * @returns true when a switch was applied.
   */
  bool handleModeSwitch(Viewport& viewport);

  /**
   * @brief Render, accumulate and publish a single progressive pass.
   *
   * The freshly rendered pass is discarded without accumulation when
   * the user closes the window or queues a mode switch mid-render, so
   * a stale frame never reaches the running average.
   *
   * @param[in,out] viewport  Viewport rendered into and published to.
   * @param[in] perPassConfig Renderer config for one pass.
   */
  void renderPass(Viewport& viewport,
                  const raytracer::core::RendererConfig& perPassConfig);

  /**
   * @brief Discard all accumulation state for a fresh render.
   *
   * Clears the radiance accumulator and the published image, and
   * resets the pass index and the completed-pass count. After this
   * call the reported progress and the exported image describe only
   * work done from this point on, never a previous viewport mode.
   */
  void resetAccumulation();

  void accumulatePass(const raytracer::components::Image& passImage);
  void publishPass(Viewport& viewport);
  void reportPassProgress(Viewport& viewport, int pass, double fraction) const;
  void reportOutcome(Viewport& viewport);

  /**
   * @brief Swap the active shading strategy to @p mode.
   *
   * Builds the matching shader via `ShadingModeFactory` and installs it
   * on the shared `ShadingContext`. The next `render()` call picks it
   * up. Callers must also discard any accumulated samples, since they
   * belong to the previous mode.
   *
   * @param[in] mode Viewport mode to switch to.
   */
  void applyViewportModeSwitch(raytracer::scene::ViewportMode mode);

  /**
   * @brief Wrap a status line with the active mode and the key hint.
   *
   * @param[in] body Core status text (e.g. "Rendering 4 / 10 samples").
   * @returns The decorated overlay line.
   */
  [[nodiscard]] std::string decorateStatus(const std::string& body) const;

  raytracer::core::RaytracerRenderer& renderer_;
  raytracer::core::RendererConfig baseConfig_;
  raytracer::core::Frame frame_;
  int width_;
  int height_;
  int targetSamples_;
  int totalPasses_;
  int effectiveSamples_;
  std::atomic<raytracer::scene::ViewportMode> currentMode_;
  std::mutex finalImageMutex_;
  raytracer::components::Image finalImage_;
  std::vector<raytracer::math::Color> accumulator_;
  raytracer::components::Image displayImage_;
  int pass_{1};
  std::atomic<int> completedPasses_{0};
  raytracer::common::Logger logger_{"ViewportLoop"};
};

}  // namespace raytracer::interface
