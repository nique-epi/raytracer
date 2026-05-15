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
#include "renderer/raytracerRenderer/RaytracerRenderer.hpp"
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

 private:
  raytracer::common::Logger logger_{"Application"};
  scene::SceneLoaderFactory _factory;
  factory::ComponentFactory _componentFactory;
};

}  // namespace raytracer::core
