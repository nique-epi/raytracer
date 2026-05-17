/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Application
*/

#pragma once

#include <memory>
#include <optional>
#include <string>

#include "application/SceneAssembler.hpp"
#include "common/helper/Logger.hpp"
#include "rendering/renderer/raytracerRenderer/RaytracerRenderer.hpp"

namespace raytracer::shading {
class ShadingPool;
}  // namespace raytracer::shading

namespace raytracer::core {

class Application {
 public:
  Application() = default;
  ~Application() = default;

  Application(const Application&) = delete;
  Application& operator=(const Application&) = delete;
  Application(Application&&) = delete;
  Application& operator=(Application&&) = delete;

  /**
   * @brief Run the render pipeline for a single scene file.
   *
   * @param [in] scenePath       Path to the scene file (`.cfg`, `.gltf`, …).
   * @param [in] useBVH          Whether to build a BVH acceleration structure.
   * @param [in] renderConfigPath Optional path to a JSON render-configuration
   *                             file. When present and compiled with
   *                             `BUILD_BONUS`, its values override the render
   *                             settings parsed from the scene file.
   * @returns 0 on success, non-zero on error.
   */
  int run(const std::string& scenePath, bool useBVH = true,
          const std::optional<std::string>& renderConfigPath = std::nullopt);

  /**
   * @brief Toggle the SFML live-display viewport for the next `run()`.
   *
   * When enabled, `run()` skips the regular blocking render and
   * delegates the progressive accumulation loop to
   * `interface::ViewportRunner`. Defaults to enabled; pass `false`
   * (CLI `--no-viewport`) for a headless render straight to `out.ppm`.
   */
  void setViewport(bool enabled);

 private:
  /**
   * @brief Run the SFML viewport accumulation loop to completion.
   *
   * @param[in] config Renderer config (scene, settings, shading context).
   * @param[in] frame  Per-frame camera state.
   * @param[in] pool   Pre-built shader pool for live mode switching.
   * @returns Viewport exit code.
   */
  static int runViewport(const RendererConfig& config, const Frame& frame,
                         std::shared_ptr<shading::ShadingPool> pool);

  /**
   * @brief Run a single blocking render and write the image to disk.
   *
   * Attaches a CLI progress bar when stderr is a TTY, runs the
   * renderer once, optionally denoises (BUILD_BONUS) and writes the
   * result to `config.outputPath` as PPM.
   *
   * @param[in] config Renderer config (scene, settings, shading context).
   * @param[in] frame  Per-frame camera state.
   * @returns 0 on success.
   */
  static int runHeadless(const RendererConfig& config, const Frame& frame);

  raytracer::common::Logger logger_{"Application"};
  SceneAssembler sceneAssembler_;
  bool viewportRequested_{true};
};

}  // namespace raytracer::core
