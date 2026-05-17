/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Application
*/

#pragma once
#include <optional>
#include <string>
#include "common/helper/Logger.hpp"
#include "factory/ComponentFactory.hpp"
#include "rendering/renderer/raytracerRenderer/RaytracerRenderer.hpp"
#include "scene/SceneLoaderFactory.hpp"
namespace raytracer::core {
class Application {
 public:
  Application();
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
  raytracer::common::Logger logger_{"Application"};
  scene::SceneLoaderFactory _factory;
  factory::ComponentFactory _componentFactory;
  bool viewportRequested_{true};
};

}  // namespace raytracer::core
