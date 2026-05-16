/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Application
*/

#pragma once
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

  int run(const std::string& scenePath, bool useBVH = true);

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
