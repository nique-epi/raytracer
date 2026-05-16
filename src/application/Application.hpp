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

#ifdef BUILD_BONUS
  /**
   * @brief Enable the SFML live-display viewport for the next `run()`.
   *
   * When set, `run()` skips the regular blocking render and delegates
   * the per-sample accumulation loop to `bonus::runWithViewport`.
   * Defaults to disabled.
   */
  void setViewport(bool enabled);
#endif

 private:
  raytracer::common::Logger logger_{"Application"};
  scene::SceneLoaderFactory _factory;
  factory::ComponentFactory _componentFactory;
#ifdef BUILD_BONUS
  bool viewportRequested_{false};
#endif
};

}  // namespace raytracer::core
