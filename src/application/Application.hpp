/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Application
*/

#pragma once
#include <string>
#include "registry/registry.hpp"
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

  int run(const std::string& scenePath);

 private:
  scene::SceneLoaderFactory _factory;
  registry::ObjectRegistry _objectRegistry;
  registry::LightRegistry _lightRegistry;
  registry::CameraRegistry _cameraRegistry;
  registry::MaterialRegistry _materialRegistry;
};

}  // namespace raytracer::core
