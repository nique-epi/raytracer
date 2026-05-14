/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Application
*/

#include "Application.hpp"
#include <memory>
#include <mutex>
#include "common/helper/Logger.hpp"
#include "components/image/Image.hpp"
#include "exceptions/Exceptions.hpp"
#include "integrator/pathIntegrator/PathIntegrator.hpp"
#include "output/ppm/ppm.hpp"
#include "renderer/Frame.hpp"
#include "renderer/RendererConfig.hpp"
#include "renderer/raytracerRenderer/RaytracerRenderer.hpp"
#include "scene/CFGSceneLoader.hpp"
#include "scene/Scene.hpp"
#include "scene/SceneBuilder.hpp"
#include "utils/math/RenderSettings.hpp"

#ifdef BUILD_BONUS
#include "Assimp/SceneLoader/AssimpLoaderRegistration.hpp"
#include "postprocess/denoise/OIDDenoiser.hpp"
#endif

namespace raytracer::core {

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
  const raytracer::common::Logger progressLogger("Progress");
  std::mutex progressMutex;
  int lastReportedPercent = -1;
  renderer.setProgressCallback([progressLogger, &progressMutex,
                                &lastReportedPercent](double progress) {
    constexpr int reportInterval = 10;
    const int percent = static_cast<int>(progress * 100.0);
    const int bucket = (percent / reportInterval) * reportInterval;
    const std::lock_guard<std::mutex> lock(progressMutex);
    if (bucket > lastReportedPercent) {
      lastReportedPercent = bucket;
      progressLogger.info("rendering ", bucket, "%");
    }
  });

  const RendererConfig config{.scene = scene,
                              .settings = settings,
                              .integrator =
                                  std::make_shared<PathIntegrator>()};
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
