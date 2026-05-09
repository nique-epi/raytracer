/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Application
*/

#include "Application.hpp"
#include <iostream>
#include <memory>
#include "Exceptions.hpp"
#include "components/image/Image.hpp"
#include "core/renderer/monoThreadRenderer/MonoThreadRenderer.hpp"
#include "output/ppm/ppm.hpp"
#include "scene/CFGSceneLoader.hpp"
#include "scene/SceneBuilder.hpp"
#include "utils/math/RenderSettings.hpp"

namespace raytracer::core {

Application::Application() {
  _factory.registerLoader(std::make_shared<scene::CFGSceneLoader>());
}

int Application::run(const std::string& scenePath) {
  _objectRegistry.loadPlugin("plugins/Sphere.so");
  _objectRegistry.loadPlugin("plugins/Plane.so");
  _objectRegistry.loadPlugin("plugins/Cylinder.so");
  _objectRegistry.loadPlugin("plugins/Cone.so");
  _lightRegistry.loadPlugin("plugins/ambient.so");
  _lightRegistry.loadPlugin("plugins/directional.so");
  _cameraRegistry.loadPlugin("plugins/raytracer_perspective_camera.so");

  const auto loader = _factory.getLoader(scenePath);
  if (!loader) {
    throw RaytracerException("No loader available for: " + scenePath);
  }

  scene::SceneBuilder builder(_objectRegistry, _lightRegistry,
                              _cameraRegistry, _materialRegistry);
  math::RenderSettings settings;

  if (!loader->load(scenePath, builder, settings)) {
    throw RaytracerException("Failed to load scene: " + scenePath);
  }

  if (!settings.validate()) {
    throw RaytracerException("Invalid render settings loaded from: " +
                             scenePath);
  }

  auto scene = builder.build();
  auto camera = scene->getCamera();
  camera->setResolution(settings.imageWidth, settings.imageHeight);

  MonoThreadRenderer renderer;
  renderer.setProgressCallback([](double progress) {
    std::cerr << "\rRendering: " << static_cast<int>(progress * 100) << "%   "
              << std::flush;
  });

  auto image = renderer.render(*scene, *camera, settings);
  std::cerr << '\n';

  output::ppm writer;
  writer.write(image, "out.ppm");
  return 0;
}

}  // namespace raytracer::core
