/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneAssembler
*/

#include "application/SceneAssembler.hpp"

#include <memory>
#include <string>
#include <utility>

#include "exceptions/Exceptions.hpp"
#include "scene/CFGSceneLoader.hpp"
#include "scene/Scene.hpp"
#include "scene/SceneBuilder.hpp"
#include "scene/World.hpp"
#include "utils/math/RenderSettings.hpp"

#ifdef BUILD_BONUS
#include "Assimp/SceneLoader/AssimpLoaderRegistration.hpp"
#include "json/JsonSettingsLoader.hpp"
#endif

namespace raytracer::core {

namespace {

std::string invalidSettingsMessage(
    const std::string& scenePath,
    const std::optional<std::string>& renderConfigPath) {
#ifdef BUILD_BONUS
  if (renderConfigPath) {
    return "Invalid render settings loaded from scene: " + scenePath +
           ", overridden by config: " + *renderConfigPath;
  }
#else
  (void)renderConfigPath;
#endif
  return "Invalid render settings loaded from: " + scenePath;
}

}  // namespace

SceneAssembler::SceneAssembler() {
  sceneLoaderFactory_.registerLoader(std::make_shared<scene::CFGSceneLoader>());

#ifdef BUILD_BONUS
  raytracer::bonus::registerAssimpLoader(sceneLoaderFactory_);
#endif
}

LoadedScene SceneAssembler::load(
    const std::string& scenePath, bool useBVH,
    const std::optional<std::string>& renderConfigPath) {
#ifndef BUILD_BONUS
  if (renderConfigPath) {
    throw RaytracerException(
        "--config requires BUILD_BONUS. Rebuild with: cmake --preset bonus");
  }
#endif

  const auto loader = sceneLoaderFactory_.getLoader(scenePath);
  if (!loader) {
    throw RaytracerException("No loader available for: " + scenePath);
  }

  scene::SceneBuilder builder(componentFactory_);
  math::RenderSettings settings;
  loader->load(scenePath, builder, settings);

  std::string outputPath = "out.ppm";
  auto scene = builder.build();

#ifdef BUILD_BONUS
  if (renderConfigPath) {
    const auto json = raytracer::bonus::json::JsonSettingsLoader::load(
        *renderConfigPath, settings);
    raytracer::bonus::json::JsonSettingsLoader::applyOverrides(
        *scene, settings, json, outputPath);
  }
#endif

  if (!settings.validate()) {
    throw RaytracerException(
        invalidSettingsMessage(scenePath, renderConfigPath));
  }

  if (useBVH) {
    scene->buildBVH();
  }
  scene->getCamera()->setResolution(settings.imageWidth, settings.imageHeight);

  return {.scene = std::move(scene),
          .settings = settings,
          .outputPath = std::move(outputPath)};
}

}  // namespace raytracer::core
