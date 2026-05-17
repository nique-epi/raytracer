/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Application
*/

#include "Application.hpp"
#include <memory>
#include <utility>
#include "application/ProgressBar.hpp"
#include "components/image/Image.hpp"
#include "interface/ViewportRunner.hpp"
#include "output/ppm/ppm.hpp"
#include "rendering/renderer/Frame.hpp"
#include "rendering/renderer/RendererConfig.hpp"
#include "rendering/renderer/raytracerRenderer/RaytracerRenderer.hpp"
#include "rendering/shading/ShadingContext.hpp"
#include "rendering/shading/ShadingPool.hpp"
#include "scene/Scene.hpp"
#include "scene/World.hpp"

#ifdef BUILD_BONUS
#include "postprocess/denoise/OIDDenoiser.hpp"
#endif

namespace raytracer::core {

int Application::run(const std::string& scenePath, bool useBVH,
                     const std::optional<std::string>& renderConfigPath) {
  auto loaded = sceneAssembler_.load(scenePath, useBVH, renderConfigPath);
  auto shadingPool = shading::ShadingPool::create();
  auto shadingContext = std::make_shared<shading::ShadingContext>(
      shadingPool->get(loaded.scene->getWorld().viewportMode()));

  const RendererConfig config{.scene = loaded.scene,
                              .settings = loaded.settings,
                              .shadingContext = shadingContext,
                              .outputPath = loaded.outputPath};
  const Frame frame{.camera = loaded.scene->getCamera()};

  return viewportRequested_ ? runViewport(config, frame, std::move(shadingPool))
                            : runHeadless(config, frame);
}

int Application::runViewport(const RendererConfig& config, const Frame& frame,
                             std::shared_ptr<shading::ShadingPool> pool) {
  RaytracerRenderer renderer;
  return raytracer::interface::ViewportRunner(renderer, config, frame,
                                              std::move(pool))
      .run();
}

int Application::runHeadless(const RendererConfig& config, const Frame& frame) {
  RaytracerRenderer renderer;
  attachConsoleProgressBar(renderer);
  components::Image image = renderer.render(config, frame);

#ifdef BUILD_BONUS
  OIDDenoiser::denoise(image);
#endif

  output::ppm writer;
  writer.write(image, config.outputPath);
  return 0;
}

void Application::setViewport(bool enabled) { viewportRequested_ = enabled; }

}  // namespace raytracer::core
