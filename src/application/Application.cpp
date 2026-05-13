/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Application
*/

#include "Application.hpp"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include "components/image/Image.hpp"
#include "exceptions/Exceptions.hpp"
#include "output/ppm/ppm.hpp"
#include "renderer/Frame.hpp"
#include "renderer/RendererConfig.hpp"
#include "renderer/monoThreadRenderer/MonoThreadRenderer.hpp"
#include "scene/CFGSceneLoader.hpp"
#include "scene/Scene.hpp"
#include "scene/SceneBuilder.hpp"
#include "utils/math/RenderSettings.hpp"

#ifdef BUILD_BONUS
#include "Assimp/SceneLoader/AssimpLoaderRegistration.hpp"
#endif

namespace raytracer::core {

namespace {

std::string formatRemainingTime(std::int64_t remainingSeconds) {
  const auto hours = remainingSeconds / 3600;
  const auto minutes = (remainingSeconds % 3600) / 60;
  const auto seconds = remainingSeconds % 60;

  std::ostringstream stream;
  if (hours > 0) {
    stream << std::setw(2) << std::setfill('0') << hours << ':';
  }
  stream << std::setw(2) << std::setfill('0') << minutes << ':' << std::setw(2)
         << std::setfill('0') << seconds;
  return stream.str();
}

void printRenderProgress(
    double progress, const std::chrono::steady_clock::time_point& renderStart) {
  const int percent = static_cast<int>(progress * 100);
  std::string remainingTime;

  if (progress > 0.0 && progress < 1.0) {
    const auto elapsedSeconds =
        std::chrono::duration<double>(std::chrono::steady_clock::now() -
                                      renderStart)
            .count();
    const auto estimatedTotalSeconds = elapsedSeconds / progress;
    const auto remainingSeconds = std::max<std::int64_t>(
        0, static_cast<std::int64_t>(estimatedTotalSeconds - elapsedSeconds));
    remainingTime = " ETA " + formatRemainingTime(remainingSeconds);
  }

  std::cerr << "\rRendering: " << percent << "%" << remainingTime << std::flush;
  if (progress >= 1.0) {
    std::cerr << "\n";
  }
}

}  // namespace

Application::Application() {
  _factory.registerLoader(std::make_shared<scene::CFGSceneLoader>());

#ifdef BUILD_BONUS
  raytracer::bonus::registerAssimpLoader(_factory);
#endif
}

int Application::run(const std::string& scenePath) {
  const auto loader = _factory.getLoader(scenePath);
  if (!loader) {
    throw RaytracerException("No loader available for: " + scenePath);
  }

  scene::SceneBuilder builder(_componentFactory);
  math::RenderSettings settings;

  loader->load(scenePath, builder, settings);

  if (!settings.validate()) {
    throw RaytracerException("Invalid render settings loaded from: " +
                             scenePath);
  }

  auto scene = builder.build();
  scene->getCamera()->setResolution(settings.imageWidth, settings.imageHeight);

  MonoThreadRenderer renderer;
  const auto renderStart = std::chrono::steady_clock::now();
  renderer.setProgressCallback([renderStart](double progress) {
    printRenderProgress(progress, renderStart);
  });

  const RendererConfig config{
      .scene = scene, .settings = settings, .integrator = nullptr};
  const Frame frame{.camera = scene->getCamera()};
  const components::Image image = renderer.render(config, frame);

  output::ppm writer;
  writer.write(image, "out.ppm");
  return 0;
}

}  // namespace raytracer::core
