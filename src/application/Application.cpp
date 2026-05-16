/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Application
*/

#include "Application.hpp"
#include <unistd.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include "components/image/Image.hpp"
#include "exceptions/Exceptions.hpp"
#include "output/ppm/ppm.hpp"
#include "rendering/integrator/whittedIntegrator/WhittedIntegrator.hpp"
#include "rendering/renderer/Frame.hpp"
#include "rendering/renderer/RendererConfig.hpp"
#include "rendering/renderer/raytracerRenderer/RaytracerRenderer.hpp"
#include "rendering/shading/IShadingMode.hpp"
#include "rendering/shading/ShadingContext.hpp"
#include "rendering/shading/materialPreview/MaterialPreviewShader.hpp"
#include "rendering/shading/rendered/RenderedShader.hpp"
#include "rendering/shading/wireframe/WireframeShader.hpp"
#include "scene/CFGSceneLoader.hpp"
#include "scene/Scene.hpp"
#include "scene/SceneBuilder.hpp"
#include "scene/World.hpp"
#include "utils/math/RenderSettings.hpp"

#ifdef BUILD_BONUS
#include "Assimp/SceneLoader/AssimpLoaderRegistration.hpp"
#include "json/RenderConfig/JsonRenderConfigLoader.hpp"
#include "postprocess/denoise/OIDDenoiser.hpp"
#endif

namespace raytracer::core {

namespace {
struct ProgressBarState {
  std::mutex mutex;
  int lastDrawnPercent{-1};
  std::chrono::steady_clock::time_point lastDrawnAt;
};

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

std::shared_ptr<shading::ShadingContext> createShadingContext(
    const scene::Scene& scene) {
  auto wireframe = std::make_shared<shading::WireframeShader>();
  auto materialPreview = std::make_shared<shading::MaterialPreviewShader>();
  auto rendered = std::make_shared<shading::RenderedShader>(
      std::make_shared<WhittedIntegrator>());

  std::shared_ptr<shading::IShadingMode> initialStrategy;
  switch (scene.getWorld().viewportMode()) {
    case scene::ViewportMode::Wireframe:
      initialStrategy = wireframe;
      break;
    case scene::ViewportMode::MaterialPreview:
      initialStrategy = materialPreview;
      break;
    case scene::ViewportMode::Rendered:
      initialStrategy = rendered;
      break;
  }

  return std::make_shared<shading::ShadingContext>(std::move(initialStrategy));
}

void displayProgressBar(RaytracerRenderer& renderer) {
  if (::isatty(::fileno(stderr)) != 0) {
    static constexpr int progressBarWidth = 30;
    const auto progressState = std::make_shared<ProgressBarState>();
    renderer.setProgressCallback([progressState](double progress) {
      const int percent = static_cast<int>(progress * 100.0);
      const std::lock_guard<std::mutex> lock(progressState->mutex);
      if (percent <= progressState->lastDrawnPercent) {
        return;
      }
      const auto now = std::chrono::steady_clock::now();
      std::string remainingText = "--:--";
      if (progressState->lastDrawnPercent >= 0 &&
          percent > progressState->lastDrawnPercent) {
        const auto elapsedSeconds =
            std::chrono::duration_cast<std::chrono::seconds>(
                now - progressState->lastDrawnAt)
                .count();
        const int percentDelta = percent - progressState->lastDrawnPercent;
        const std::int64_t remainingSeconds =
            static_cast<std::int64_t>(elapsedSeconds) * (100 - percent) /
            percentDelta;
        remainingText = formatRemainingTime(remainingSeconds);
      }
      progressState->lastDrawnPercent = percent;
      progressState->lastDrawnAt = now;
      const int filled = (percent * progressBarWidth) / 100;
      std::cerr << "\rRendering [";
      for (int i = 0; i < progressBarWidth; ++i) {
        std::cerr << (i < filled ? "█" : "░");
      }
      std::cerr << "] " << std::setw(3) << percent
                << "% remaining=" << remainingText << std::flush;
      if (progress >= 1.0) {
        std::cerr << '\n';
      }
    });
  }
}

}  // namespace

Application::Application() {
  _factory.registerLoader(std::make_shared<scene::CFGSceneLoader>());

#ifdef BUILD_BONUS
  raytracer::bonus::registerAssimpLoader(_factory);
#endif
}

int Application::run(const std::string& scenePath, bool useBVH,
                     const std::optional<std::string>& renderConfigPath) {
#ifndef BUILD_BONUS
  if (renderConfigPath) {
    throw RaytracerException(
        "--config requires BUILD_BONUS. Rebuild with: cmake --preset bonus");
  }
#endif
  const auto loader = _factory.getLoader(scenePath);
  if (!loader) {
    throw RaytracerException("No loader available for: " + scenePath);
  }

  scene::SceneBuilder builder(_componentFactory);
  math::RenderSettings settings;

  loader->load(scenePath, builder, settings);

#ifdef BUILD_BONUS
  std::optional<raytracer::bonus::json::GlobalRenderConfig> globalConfig;
  if (renderConfigPath) {
    globalConfig = raytracer::bonus::json::JsonRenderConfigLoader::load(
        *renderConfigPath, settings);
    settings = globalConfig->settings;
  }
#endif

  if (!settings.validate()) {
    throw RaytracerException("Invalid render settings loaded from: " +
                             scenePath);
  }
  auto scene = builder.build();

#ifdef BUILD_BONUS
  if (globalConfig && globalConfig->viewportMode) {
    scene->getWorld().setViewportMode(*globalConfig->viewportMode);
  }
#endif
  if (useBVH) {
    scene->buildBVH();
  }
  scene->getCamera()->setResolution(settings.imageWidth, settings.imageHeight);

  RaytracerRenderer renderer;
  displayProgressBar(renderer);
  auto shadingContext = createShadingContext(*scene);

  const RendererConfig config{
      .scene = scene, .settings = settings, .shadingContext = shadingContext};
  const Frame frame{.camera = scene->getCamera()};
  components::Image image = renderer.render(config, frame);

  output::ppm writer;

#ifdef BUILD_BONUS
  OIDDenoiser::denoise(image);
#endif
  writer.write(image, "out.ppm");
  return 0;
}

}  // namespace raytracer::core
