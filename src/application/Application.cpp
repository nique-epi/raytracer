/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Application
*/

#include "Application.hpp"
#include <iostream>
#include <memory>
#include "exceptions/Exceptions.hpp"
#include "components/image/Image.hpp"
#include "output/ppm/ppm.hpp"
#include "integrator/pathIntegrator/PathIntegrator.hpp"
#include "renderer/Frame.hpp"
#include "renderer/RendererConfig.hpp"
#include "renderer/monoThreadRenderer/MonoThreadRenderer.hpp"
#include "scene/CFGSceneLoader.hpp"
#include "scene/SceneBuilder.hpp"
#include "utils/math/RenderSettings.hpp"

namespace raytracer::core {

Application::Application() {
  _factory.registerLoader(std::make_shared<scene::CFGSceneLoader>());
}

int Application::run(const std::string& scenePath) {
  const auto loader = _factory.getLoader(scenePath);
  if (!loader) {
    throw RaytracerException("No loader available for: " + scenePath);
  }

  scene::SceneBuilder builder(_componentFactory);
  math::RenderSettings settings;

  if (!loader->load(scenePath, builder, settings)) {
    throw RaytracerException("Failed to load scene: " + scenePath);
  }

  if (!settings.validate()) {
    throw RaytracerException("Invalid render settings loaded from: " +
                             scenePath);
  }

  auto scene = builder.build();
  scene->getCamera()->setResolution(settings.imageWidth, settings.imageHeight);

  MonoThreadRenderer renderer;
  renderer.setProgressCallback([](double progress) {
    const int percent = static_cast<int>(progress * 100);
    std::cerr << "\rRendering: " << percent << "%" << std::flush;
    if (progress >= 1.0) {
      std::cerr << "\n";
    }
  });

  const RendererConfig config{
      .scene = *scene,
      .settings = settings,
      .integrator = std::make_shared<PathIntegrator>()};
  const Frame frame{.camera = *scene->getCamera()};
  const components::Image image = renderer.render(config, frame);

  output::ppm writer;
  writer.write(image, "out.ppm");
  return 0;
}

}  // namespace raytracer::core
