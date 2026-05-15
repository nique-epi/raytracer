/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Application
*/

#include "Application.hpp"
#include <unistd.h>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
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
#include "postprocess/denoise/OIDDenoiser.hpp"
#endif

namespace raytracer::core {

namespace {
struct ProgressBarState {
  std::mutex mutex;
  int lastDrawnPercent{-1};
};

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
      progressState->lastDrawnPercent = percent;
      const int filled = (percent * progressBarWidth) / 100;
      std::cerr << "\rRendering [";
      for (int i = 0; i < progressBarWidth; ++i) {
        std::cerr << (i < filled ? "█" : "░");
      }
      std::cerr << "] " << std::setw(3) << percent << "%" << std::flush;
      if (progress >= 1.0) {
        std::cerr << '\n';
      }
    });
  }
}

void hdrToSdr(components::Image& image) {
  for (std::size_t y = 0; y < image.getHeight(); ++y) {
    for (std::size_t x = 0; x < image.getWidth(); ++x) {
      auto c = image.getPixel(x, y);

      c.r = c.r / (c.r + 1.0);
      c.g = c.g / (c.g + 1.0);
      c.b = c.b / (c.b + 1.0);

      c.r = std::pow(c.r, 1.0 / 2.2);
      c.g = std::pow(c.g, 1.0 / 2.2);
      c.b = std::pow(c.b, 1.0 / 2.2);

      image.setPixel(x, y, c);
    }
  }
}

}  // namespace

Application::Application() {
  _factory.registerLoader(std::make_shared<scene::CFGSceneLoader>());

#ifdef BUILD_BONUS
  raytracer::bonus::registerAssimpLoader(_factory);
#endif
}

int Application::run(const std::string& scenePath, bool useBVH) {
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
  hdrToSdr(image);
#ifdef BUILD_BONUS
  OIDDenoiser::denoise(image);
#endif
  writer.write(image, "out.ppm");
  return 0;
}

}  // namespace raytracer::core
