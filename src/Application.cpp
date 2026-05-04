/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Application
*/

#include "Application.hpp"
#include <iostream>
#include <memory>
#include "components/image/Image.hpp"
#include "output/ppm/ppm.hpp"
#include "scene/CFGSceneLoader.hpp"
#include "scene/SceneBuilder.hpp"
#include "utils/math/RenderSettings.hpp"

namespace raytracer {

Application::Application() {
  _factory.registerLoader(std::make_shared<scene::CFGSceneLoader>());
}

int Application::run(const std::string& scenePath) {
  const auto loader = _factory.getLoader(scenePath);
  if (!loader) {
    std::cerr << "No loader available for: " << scenePath << "\n";
    return ErrorExitCode;
  }

  scene::SceneBuilder builder;
  math::RenderSettings settings;

  if (!loader->load(scenePath, builder, settings)) {
    return ErrorExitCode;
  }

  components::Image image(settings.imageWidth, settings.imageHeight);
  output::ppm writer;
  writer.write(image, "out.ppm");
  return 0;
}

}  // namespace raytracer
