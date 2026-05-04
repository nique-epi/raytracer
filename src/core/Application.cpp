/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Application
*/

#include "Application.hpp"
#include <memory>
#include "Exceptions.hpp"
#include "components/image/Image.hpp"
#include "output/ppm/ppm.hpp"
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

  scene::SceneBuilder builder;
  math::RenderSettings settings;

  if (!loader->load(scenePath, builder, settings)) {
    throw RaytracerException("Failed to load scene: " + scenePath);
  }

  if (!settings.validate()) {
    throw RaytracerException("Invalid render settings loaded from: " +
                             scenePath);
  }

  components::Image image(settings.imageWidth, settings.imageHeight);
  output::ppm writer;
  writer.write(image, "out.ppm");
  return 0;
}

}  // namespace raytracer::core
